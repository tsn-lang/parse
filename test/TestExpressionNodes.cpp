#include "Common.h"
#include <parse/expressions/ArrowFunctionExpressionNode.h>
#include <parse/expressions/AwaitExpressionNode.h>
#include <parse/expressions/BinaryExpressionNode.h>
#include <parse/expressions/CallExpressionNode.h>
#include <parse/expressions/CastExpressionNode.h>
#include <parse/expressions/ConditionalExpressionNode.h>
#include <parse/expressions/ExpressionSequenceNode.h>
#include <parse/expressions/FunctionExpressionNode.h>
#include <parse/expressions/IndexExpressionNode.h>
#include <parse/expressions/MemberExpressionNode.h>
#include <parse/expressions/NewExpressionNode.h>
#include <parse/expressions/SizeOfExpressionNode.h>
#include <parse/expressions/TypeInfoExpressionNode.h>
#include <parse/expressions/UnaryExpressionNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/proxies/StatementNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <parse/misc/ParameterListNode.h>
#include <parse/misc/IdentifierNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/types/IdentifierTypeSpecifierNode.h>
#include <parse/literals/NumberLiteralNode.h>
#include <utils/Array.hpp>

using namespace tokenize;
using namespace parse;

TEST_CASE("Test ArrowFunctionExpressionNode", "[parse]") {
    SECTION("parameters") {
        TestContext test0("() => 5;");
        ArrowFunctionExpressionNode* n = nullptr;
        
        n = ArrowFunctionExpressionNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 0);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);
        
        TestContext test1("(p1: i32, p2: i32) => p1 + p2;");
        n = ArrowFunctionExpressionNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 2);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }

    SECTION("return type") {
        TestContext test0("() => 5;");
        ArrowFunctionExpressionNode* n = nullptr;
        
        n = ArrowFunctionExpressionNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->returnType == nullptr);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);
        
        TestContext test1("() : i32 => 5;");
        n = ArrowFunctionExpressionNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->returnType != nullptr);
        REQUIRE(n->returnType->type != nullptr);
        REQUIRE(n->returnType->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->returnType->type)->name != nullptr);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->returnType->type)->name->text == "i32");
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }

    SECTION("body") {
        TestContext test0("() => 5;");
        ArrowFunctionExpressionNode* n = nullptr;
        
        n = ArrowFunctionExpressionNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->body != nullptr);
        REQUIRE(n->body->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)n->body)->expr != nullptr);
        REQUIRE(((ExpressionNode*)n->body)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);
        
        TestContext test1("() => { return 5; };");
        n = ArrowFunctionExpressionNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->body->getType() == NodeType::StatementBlockNode);
        REQUIRE(((StatementBlockNode*)n->body)->statements.size() == 1);
        REQUIRE(((StatementBlockNode*)n->body)->statements[0] != nullptr);
        REQUIRE(((StatementBlockNode*)n->body)->statements[0]->stmt != nullptr);
        REQUIRE(((StatementBlockNode*)n->body)->statements[0]->stmt->getType() == NodeType::ReturnStatementNode);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
}

TEST_CASE("Test AwaitExpressionNode", "[parse]") {
    TestContext test("await test;");
    Node* n = nullptr;
    
    n = AwaitExpressionNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->getType() == NodeType::AwaitExpressionNode);
    REQUIRE(n->isError() == false);
    REQUIRE(((AwaitExpressionNode*)n)->expr != nullptr);
    REQUIRE(((AwaitExpressionNode*)n)->expr->getType() == NodeType::IdentifierNode);
    REQUIRE(test.ctx->getState()->messages.size() == 0);
}

TEST_CASE("Test BinaryExpressionNode", "[parse]") {
    SECTION("exponent expression") {
        TestContext test("2 ** 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseExponentExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::Exp);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("multiply expression") {
        TestContext test("2 * 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseMultiplicativeExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::Mul);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("divide expression") {
        TestContext test("2 / 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseMultiplicativeExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::Div);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("modulo expression") {
        TestContext test("2 % 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseMultiplicativeExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::Mod);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("add expression") {
        TestContext test("2 + 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseAdditiveExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::Add);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("subtract expression") {
        TestContext test("2 - 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseAdditiveExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::Sub);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("shift left expression") {
        TestContext test("2 << 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseBitShiftExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::ShiftLeft);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("shift right expression") {
        TestContext test("2 >> 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseBitShiftExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::ShiftRight);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("less than expression") {
        TestContext test("2 < 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseRelationalExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::LessThan);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("less than or equal to expression") {
        TestContext test("2 <= 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseRelationalExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::LessThanEq);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("greater than expression") {
        TestContext test("2 > 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseRelationalExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::GreaterThan);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("greater than or equal to expression") {
        TestContext test("2 >= 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseRelationalExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::GreaterThanEq);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("equality expression") {
        TestContext test("2 == 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseEqualityExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::Equality);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("inequality expression") {
        TestContext test("2 != 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseEqualityExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::Inequality);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("bitwise and expression") {
        TestContext test("2 & 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseBitwiseAndExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::BitwiseAnd);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("bitwise xor expression") {
        TestContext test("2 ^ 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseBitwiseXorExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::BitwiseXOr);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("bitwise or expression") {
        TestContext test("2 | 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseBitwiseOrExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::BitwiseOr);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("logical and expression") {
        TestContext test("2 && 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseLogicalAndExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::LogicalAnd);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("logical or expression") {
        TestContext test("2 || 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseLogicalOrExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::LogicalOr);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("assignment expression") {
        TestContext test("2 = 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseAssignmentExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::Assign);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("(assignment) exponent expression") {
        TestContext test("2 **= 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseAssignmentOperatorExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::ExpEq);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("(assignment) multiply expression") {
        TestContext test("2 *= 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseAssignmentOperatorExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::MulEq);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("(assignment) divide expression") {
        TestContext test("2 /= 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseAssignmentOperatorExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::DivEq);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("(assignment) modulo expression") {
        TestContext test("2 %= 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseAssignmentOperatorExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::ModEq);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("(assignment) add expression") {
        TestContext test("2 += 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseAssignmentOperatorExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::AddEq);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("(assignment) subtract expression") {
        TestContext test("2 -= 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseAssignmentOperatorExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::SubEq);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("(assignment) shift left expression") {
        TestContext test("2 <<= 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseAssignmentOperatorExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::ShiftLeftEq);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("(assignment) shift right expression") {
        TestContext test("2 >>= 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseAssignmentOperatorExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::ShiftRightEq);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("(assignment) bitwise and expression") {
        TestContext test("2 &= 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseAssignmentOperatorExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::BitwiseAndEq);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("(assignment) bitwise xor expression") {
        TestContext test("2 ^= 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseAssignmentOperatorExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::BitwiseXOrEq);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("(assignment) bitwise or expression") {
        TestContext test("2 |= 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseAssignmentOperatorExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::BitwiseOrEq);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("(assignment) logical and expression") {
        TestContext test("2 &&= 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseAssignmentOperatorExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::LogicalAndEq);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("(assignment) logical or expression") {
        TestContext test("2 ||= 4");
        Node* n = nullptr;
        
        n = BinaryExpressionNode::TryParseAssignmentOperatorExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->isError() == false);

        BinaryExpressionNode* b = (BinaryExpressionNode*)n;
        REQUIRE(b->operation == OperatorType::LogicalOrEq);
        
        REQUIRE(b->lhs != nullptr);
        REQUIRE(b->lhs->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)b->lhs)->value.s == 2);

        REQUIRE(b->rhs != nullptr);
        REQUIRE(b->rhs->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)b->rhs)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)b->rhs)->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
}

TEST_CASE("Test CallExpressionNode", "[parse]") {
    SECTION("callee") {
        TestContext test("test()");
        Node* n = nullptr;
        
        n = CallExpressionNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::CallExpressionNode);
        REQUIRE(n->isError() == false);
        
        CallExpressionNode* c = (CallExpressionNode*)n;
        REQUIRE(c->callee != nullptr);
        REQUIRE(c->callee->getType() == NodeType::IdentifierNode);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("arguments") {
        Node* n = nullptr;
        CallExpressionNode* c = nullptr;
        
        TestContext test0("test()");
        n = CallExpressionNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::CallExpressionNode);
        REQUIRE(n->isError() == false);
        
        c = (CallExpressionNode*)n;
        REQUIRE(c->arguments == nullptr);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);
        
        TestContext test1("test(1, 2, 3)");
        n = CallExpressionNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::CallExpressionNode);
        REQUIRE(n->isError() == false);
        
        c = (CallExpressionNode*)n;
        REQUIRE(c->arguments != nullptr);
        REQUIRE(c->arguments->expressions.size() == 3);

        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }

    SECTION("nesting") {
        struct TestCase {
            const char* codeStr;
            Array<NodeType> types;
        };

        TestCase tests[] = {
            {
                "test().prop()",
                {
                    NodeType::CallExpressionNode,
                    NodeType::MemberExpressionNode,
                    NodeType::CallExpressionNode
                }
            }, {
                "test().prop1().prop2()",
                {
                    NodeType::CallExpressionNode,
                    NodeType::MemberExpressionNode,
                    NodeType::CallExpressionNode,
                    NodeType::MemberExpressionNode,
                    NodeType::CallExpressionNode
                }
            }, {
                "test().prop1.prop2()",
                {
                    NodeType::CallExpressionNode,
                    NodeType::MemberExpressionNode,
                    NodeType::MemberExpressionNode,
                    NodeType::CallExpressionNode
                }
            }, {
                "test.prop1.prop2()",
                {
                    NodeType::CallExpressionNode,
                    NodeType::MemberExpressionNode,
                    NodeType::MemberExpressionNode,
                    NodeType::IdentifierNode
                }
            }, {
                "test()[0]()",
                {
                    NodeType::CallExpressionNode,
                    NodeType::IndexExpressionNode,
                    NodeType::CallExpressionNode
                }
            }, {
                "test()[0]()[1]()",
                {
                    NodeType::CallExpressionNode,
                    NodeType::IndexExpressionNode,
                    NodeType::CallExpressionNode,
                    NodeType::IndexExpressionNode,
                    NodeType::CallExpressionNode
                }
            }, {
                "test()[0][1]()",
                {
                    NodeType::CallExpressionNode,
                    NodeType::IndexExpressionNode,
                    NodeType::IndexExpressionNode,
                    NodeType::CallExpressionNode
                }
            }, {
                "test[0][1]()",
                {
                    NodeType::CallExpressionNode,
                    NodeType::IndexExpressionNode,
                    NodeType::IndexExpressionNode,
                    NodeType::IdentifierNode
                }
            }
        };

        u32 testCount = sizeof(tests) / sizeof(TestCase);
        for (u32 i = 0;i < testCount;i++) {
            CAPTURE(i);
            CAPTURE(tests[i].codeStr);
            TestContext test(tests[i].codeStr);
            Node* n = CallExpressionNode::TryParse(test.ctx);
            REQUIRE(test.ctx->getState()->messages.size() == 0);

            for (u32 t = 0;t < tests[i].types.size();t++) {
                CAPTURE(t);
                REQUIRE(n != nullptr);
                REQUIRE(n->getType() == tests[i].types[t]);
                REQUIRE(n->isError() == false);
                
                switch (tests[i].types[t]) {
                    case NodeType::CallExpressionNode: {
                        CallExpressionNode* c = (CallExpressionNode*)n;
                        REQUIRE(c->callee != nullptr);
                        n = c->callee;
                        break;
                    }
                    case NodeType::MemberExpressionNode: {
                        MemberExpressionNode* m = (MemberExpressionNode*)n;
                        REQUIRE(m->object != nullptr);
                        n = m->object;
                        break;
                    }
                    case NodeType::IndexExpressionNode: {
                        IndexExpressionNode* x = (IndexExpressionNode*)n;
                        REQUIRE(x->object != nullptr);
                        n = x->object;
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        }
    }
}

TEST_CASE("Test CastExpressionNode", "[parse]") {
    TestContext test("test as i32;");
    Node* n = CastExpressionNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->getType() == NodeType::CastExpressionNode);
    REQUIRE(n->isError() == false);

    CastExpressionNode* c = (CastExpressionNode*)n;
    REQUIRE(c->expr != nullptr);
    REQUIRE(c->expr->getType() == NodeType::IdentifierNode);
    REQUIRE(((IdentifierNode*)c->expr)->text == "test");
    REQUIRE(c->asType != nullptr);
    REQUIRE(c->asType->type != nullptr);
    REQUIRE(c->asType->type->getType() == NodeType::IdentifierTypeSpecifierNode);
    REQUIRE(((IdentifierTypeSpecifierNode*)c->asType->type)->name != nullptr);
    REQUIRE(((IdentifierTypeSpecifierNode*)c->asType->type)->name->text == "i32");
    REQUIRE(test.ctx->getState()->messages.size() == 0);
}

TEST_CASE("Test ConditionalExpressionNode", "[parse]") {
    TestContext test("test ? 1 : 2;");
    Node* n = ConditionalExpressionNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->getType() == NodeType::ConditionalExpressionNode);
    REQUIRE(n->isError() == false);

    ConditionalExpressionNode* c = (ConditionalExpressionNode*)n;
    REQUIRE(c->condition != nullptr);
    REQUIRE(c->condition->getType() == NodeType::IdentifierNode);
    REQUIRE(((IdentifierNode*)c->condition)->text == "test");

    REQUIRE(c->valueOnTrue != nullptr);
    REQUIRE(c->valueOnTrue->getType() == NodeType::ExpressionNode);
    REQUIRE(((ExpressionNode*)c->valueOnTrue)->expr->getType() == NodeType::NumberLiteralNode);
    REQUIRE(((NumberLiteralNode*)((ExpressionNode*)c->valueOnTrue)->expr)->value.s == 1);

    REQUIRE(c->valueOnFalse != nullptr);
    REQUIRE(c->valueOnFalse->getType() == NodeType::ExpressionNode);
    REQUIRE(((ExpressionNode*)c->valueOnFalse)->expr->getType() == NodeType::NumberLiteralNode);
    REQUIRE(((NumberLiteralNode*)((ExpressionNode*)c->valueOnFalse)->expr)->value.s == 2);

    REQUIRE(test.ctx->getState()->messages.size() == 0);
}

TEST_CASE("Test ExpressionSequenceNode", "[parse]") {
    SECTION("single unparenthesized") {
        TestContext test("3 + 1");
        Node* n = ExpressionSequenceNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::ExpressionSequenceNode);
        REQUIRE(n->isError() == false);

        ExpressionSequenceNode* s = (ExpressionSequenceNode*)n;
        REQUIRE(s->expressions.size() == 1);

        REQUIRE(s->expressions[0] != nullptr);
        REQUIRE(s->expressions[0]->expr != nullptr);
        REQUIRE(s->expressions[0]->expr->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(((BinaryExpressionNode*)s->expressions[0]->expr)->operation == OperatorType::Add);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("single parenthesized") {
        TestContext test("(3 + 1)");
        Node* n = ExpressionSequenceNode::TryParseParenthesized(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::ExpressionSequenceNode);
        REQUIRE(n->isError() == false);

        ExpressionSequenceNode* s = (ExpressionSequenceNode*)n;
        REQUIRE(s->expressions.size() == 1);

        REQUIRE(s->expressions[0] != nullptr);
        REQUIRE(s->expressions[0]->expr != nullptr);
        REQUIRE(s->expressions[0]->expr->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(((BinaryExpressionNode*)s->expressions[0]->expr)->operation == OperatorType::Add);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("multiple unparenthesized") {
        TestContext test("1, 2, 3 + 1");
        Node* n = ExpressionSequenceNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::ExpressionSequenceNode);
        REQUIRE(n->isError() == false);

        ExpressionSequenceNode* s = (ExpressionSequenceNode*)n;
        REQUIRE(s->expressions.size() == 3);

        REQUIRE(s->expressions[0] != nullptr);
        REQUIRE(s->expressions[0]->expr != nullptr);
        REQUIRE(s->expressions[0]->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(s->expressions[1] != nullptr);
        REQUIRE(s->expressions[1]->expr != nullptr);
        REQUIRE(s->expressions[1]->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(s->expressions[2] != nullptr);
        REQUIRE(s->expressions[2]->expr != nullptr);
        REQUIRE(s->expressions[2]->expr->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(((BinaryExpressionNode*)s->expressions[2]->expr)->operation == OperatorType::Add);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("multiple parenthesized") {
        TestContext test("(1, 2, 3 + 1)");
        Node* n = ExpressionSequenceNode::TryParseParenthesized(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::ExpressionSequenceNode);
        REQUIRE(n->isError() == false);

        ExpressionSequenceNode* s = (ExpressionSequenceNode*)n;
        REQUIRE(s->expressions.size() == 3);

        REQUIRE(s->expressions[0] != nullptr);
        REQUIRE(s->expressions[0]->expr != nullptr);
        REQUIRE(s->expressions[0]->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(s->expressions[1] != nullptr);
        REQUIRE(s->expressions[1]->expr != nullptr);
        REQUIRE(s->expressions[1]->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(s->expressions[2] != nullptr);
        REQUIRE(s->expressions[2]->expr != nullptr);
        REQUIRE(s->expressions[2]->expr->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(((BinaryExpressionNode*)s->expressions[2]->expr)->operation == OperatorType::Add);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
}

TEST_CASE("Test FunctionExpressionNode", "[parse]") {
    SECTION("parameters") {
        TestContext test0("function() { return 5; }");
        FunctionExpressionNode* n = nullptr;
        
        n = FunctionExpressionNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 0);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);
        
        TestContext test1("function(p1: i32, p2: i32) { return p1 + p2; }");
        n = FunctionExpressionNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 2);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }

    SECTION("return type") {
        TestContext test0("function() { return 5; }");
        FunctionExpressionNode* n = nullptr;
        
        n = FunctionExpressionNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->returnType == nullptr);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);
        
        TestContext test1("function() : i32 { return 5; }");
        n = FunctionExpressionNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->returnType != nullptr);
        REQUIRE(n->returnType->type != nullptr);
        REQUIRE(n->returnType->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->returnType->type)->name != nullptr);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->returnType->type)->name->text == "i32");
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }

    SECTION("body") {
        TestContext test("function() { return 5; }");
        FunctionExpressionNode* n = FunctionExpressionNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->body->getType() == NodeType::StatementBlockNode);
        REQUIRE(((StatementBlockNode*)n->body)->statements.size() == 1);
        REQUIRE(((StatementBlockNode*)n->body)->statements[0] != nullptr);
        REQUIRE(((StatementBlockNode*)n->body)->statements[0]->stmt != nullptr);
        REQUIRE(((StatementBlockNode*)n->body)->statements[0]->stmt->getType() == NodeType::ReturnStatementNode);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
}

TEST_CASE("Test IndexExpressionNode", "[parse]") {
    SECTION("object") {
        TestContext test("test[0]");
        Node* n = nullptr;
        
        n = IndexExpressionNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::IndexExpressionNode);
        REQUIRE(n->isError() == false);
        
        IndexExpressionNode* c = (IndexExpressionNode*)n;
        REQUIRE(c->object != nullptr);
        REQUIRE(c->object->getType() == NodeType::IdentifierNode);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("index") {
        Node* n = nullptr;
        IndexExpressionNode* c = nullptr;
        
        TestContext test0("test[0]");
        n = IndexExpressionNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::IndexExpressionNode);
        REQUIRE(n->isError() == false);
        
        c = (IndexExpressionNode*)n;
        REQUIRE(c->index != nullptr);
        REQUIRE(c->index->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)c->index)->expr != nullptr);
        REQUIRE(((ExpressionNode*)c->index)->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);
        
        TestContext test1("test[a + b]");
        n = IndexExpressionNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::IndexExpressionNode);
        REQUIRE(n->isError() == false);
        
        c = (IndexExpressionNode*)n;
        REQUIRE(c->index != nullptr);
        REQUIRE(c->index->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)c->index)->expr != nullptr);
        REQUIRE(((ExpressionNode*)c->index)->expr->getType() == NodeType::BinaryExpressionNode);

        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }

    SECTION("nesting") {
        struct TestCase {
            const char* codeStr;
            Array<NodeType> types;
        };

        TestCase tests[] = {
            {
                "test[0].prop",
                {
                    NodeType::MemberExpressionNode,
                    NodeType::IndexExpressionNode,
                    NodeType::IdentifierNode
                }
            }, {
                "test.prop[0]",
                {
                    NodeType::IndexExpressionNode,
                    NodeType::MemberExpressionNode,
                    NodeType::IdentifierNode
                }
            }, {
                "test[0][1][2]",
                {
                    NodeType::IndexExpressionNode,
                    NodeType::IndexExpressionNode,
                    NodeType::IndexExpressionNode,
                    NodeType::IdentifierNode
                }
            }, {
                "test.prop1.prop2",
                {
                    NodeType::MemberExpressionNode,
                    NodeType::MemberExpressionNode,
                    NodeType::IdentifierNode
                }
            }
        };

        u32 testCount = sizeof(tests) / sizeof(TestCase);
        for (u32 i = 0;i < testCount;i++) {
            CAPTURE(i);
            CAPTURE(tests[i].codeStr);
            TestContext test(tests[i].codeStr);
            Node* n = IndexExpressionNode::TryParse(test.ctx);
            REQUIRE(test.ctx->getState()->messages.size() == 0);

            for (u32 t = 0;t < tests[i].types.size();t++) {
                CAPTURE(t);
                REQUIRE(n != nullptr);
                REQUIRE(n->getType() == tests[i].types[t]);
                REQUIRE(n->isError() == false);
                
                switch (tests[i].types[t]) {
                    case NodeType::MemberExpressionNode: {
                        MemberExpressionNode* m = (MemberExpressionNode*)n;
                        REQUIRE(m->object != nullptr);
                        n = m->object;
                        break;
                    }
                    case NodeType::IndexExpressionNode: {
                        IndexExpressionNode* x = (IndexExpressionNode*)n;
                        REQUIRE(x->object != nullptr);
                        n = x->object;
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        }
    }
}

TEST_CASE("Test MemberExpressionNode", "[parse]") {
    SECTION("object") {
        TestContext test("test.prop");
        Node* n = nullptr;
        
        n = MemberExpressionNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::MemberExpressionNode);
        REQUIRE(n->isError() == false);
        
        MemberExpressionNode* c = (MemberExpressionNode*)n;
        REQUIRE(c->object != nullptr);
        REQUIRE(c->object->getType() == NodeType::IdentifierNode);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("property name") {
        Node* n = nullptr;
        MemberExpressionNode* c = nullptr;
        
        TestContext test("test.prop");
        n = MemberExpressionNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::MemberExpressionNode);
        REQUIRE(n->isError() == false);
        
        c = (MemberExpressionNode*)n;
        REQUIRE(c->propertyName == "prop");
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("nesting") {
        struct TestCase {
            const char* codeStr;
            Array<NodeType> types;
        };

        TestCase tests[] = {
            {
                "test[0].prop",
                {
                    NodeType::MemberExpressionNode,
                    NodeType::IndexExpressionNode,
                    NodeType::IdentifierNode
                }
            }, {
                "test.prop[0]",
                {
                    NodeType::IndexExpressionNode,
                    NodeType::MemberExpressionNode,
                    NodeType::IdentifierNode
                }
            }, {
                "test[0][1][2]",
                {
                    NodeType::IndexExpressionNode,
                    NodeType::IndexExpressionNode,
                    NodeType::IndexExpressionNode,
                    NodeType::IdentifierNode
                }
            }, {
                "test.prop1.prop2",
                {
                    NodeType::MemberExpressionNode,
                    NodeType::MemberExpressionNode,
                    NodeType::IdentifierNode
                }
            }
        };

        u32 testCount = sizeof(tests) / sizeof(TestCase);
        for (u32 i = 0;i < testCount;i++) {
            CAPTURE(i);
            CAPTURE(tests[i].codeStr);
            TestContext test(tests[i].codeStr);
            Node* n = MemberExpressionNode::TryParse(test.ctx);
            REQUIRE(test.ctx->getState()->messages.size() == 0);

            for (u32 t = 0;t < tests[i].types.size();t++) {
                CAPTURE(t);
                REQUIRE(n != nullptr);
                REQUIRE(n->getType() == tests[i].types[t]);
                REQUIRE(n->isError() == false);
                
                switch (tests[i].types[t]) {
                    case NodeType::MemberExpressionNode: {
                        MemberExpressionNode* m = (MemberExpressionNode*)n;
                        REQUIRE(m->object != nullptr);
                        n = m->object;
                        break;
                    }
                    case NodeType::IndexExpressionNode: {
                        IndexExpressionNode* x = (IndexExpressionNode*)n;
                        REQUIRE(x->object != nullptr);
                        n = x->object;
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        }
    }
}

TEST_CASE("Test NewExpressionNode", "[parse]") {
    SECTION("type") {
        TestContext test("new Type()");
        Node* n = nullptr;
        
        n = NewExpressionNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NewExpressionNode);
        REQUIRE(n->isError() == false);
        
        NewExpressionNode* e = (NewExpressionNode*)n;
        REQUIRE(e->type != nullptr);
        REQUIRE(e->type->type != nullptr);
        REQUIRE(e->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)e->type->type)->name != nullptr);
        REQUIRE(((IdentifierTypeSpecifierNode*)e->type->type)->name->text == "Type");

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("arguments") {
        Node* n = nullptr;
        NewExpressionNode* e = nullptr;
        
        TestContext test0("new Type()");
        n = NewExpressionNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NewExpressionNode);
        REQUIRE(n->isError() == false);
        
        e = (NewExpressionNode*)n;
        REQUIRE(e->arguments == nullptr);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);
        
        TestContext test1("new Type(1, 2, 3)");
        n = NewExpressionNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NewExpressionNode);
        REQUIRE(n->isError() == false);
        
        e = (NewExpressionNode*)n;
        REQUIRE(e->arguments != nullptr);
        REQUIRE(e->arguments->expressions.size() == 3);

        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("destination") {
        Node* n = nullptr;
        NewExpressionNode* e = nullptr;
        
        TestContext test0("new Type()");
        n = NewExpressionNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NewExpressionNode);
        REQUIRE(n->isError() == false);
        
        e = (NewExpressionNode*)n;
        REQUIRE(e->destination == nullptr);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);
        
        TestContext test1("new Type() => mem");
        n = NewExpressionNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NewExpressionNode);
        REQUIRE(n->isError() == false);
        
        e = (NewExpressionNode*)n;
        REQUIRE(e->destination != nullptr);
        REQUIRE(e->destination->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)e->destination)->expr != nullptr);
        REQUIRE(((ExpressionNode*)e->destination)->expr->getType() == NodeType::IdentifierNode);
        REQUIRE(((IdentifierNode*)((ExpressionNode*)e->destination)->expr)->text == "mem");

        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
}

TEST_CASE("Test SizeOfExpressionNode", "[parse]") {
    TestContext test("sizeof<Test>");
    Node* n = nullptr;
    
    n = SizeOfExpressionNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->getType() == NodeType::SizeOfExpressionNode);
    REQUIRE(n->isError() == false);

    SizeOfExpressionNode* s = (SizeOfExpressionNode*)n;
    REQUIRE(s->type != nullptr);
    REQUIRE(s->type->getType() == NodeType::TypeSpecifierNode);
    REQUIRE(test.ctx->getState()->messages.size() == 0);
}

// ThisExpressionNode intentionally left out because it doesn't need a TryParse method

TEST_CASE("Test TypeInfoExpressionNode", "[parse]") {
    TestContext test("typeinfo<Test>");
    Node* n = nullptr;
    
    n = TypeInfoExpressionNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->getType() == NodeType::TypeInfoExpressionNode);
    REQUIRE(n->isError() == false);

    TypeInfoExpressionNode* s = (TypeInfoExpressionNode*)n;
    REQUIRE(s->type != nullptr);
    REQUIRE(s->type->getType() == NodeType::TypeSpecifierNode);
    REQUIRE(test.ctx->getState()->messages.size() == 0);
}

TEST_CASE("Test UnaryExpressionNode", "[parse]") {
    SECTION("postfix increment") {
        TestContext test("4++");
        Node* n = nullptr;
        
        n = UnaryExpressionNode::TryParsePrefixUnaryExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::UnaryExpressionNode);
        REQUIRE(n->isError() == false);

        UnaryExpressionNode* u = (UnaryExpressionNode*)n;
        REQUIRE(u->operation == OperatorType::PostIncrement);
        
        REQUIRE(u->expr != nullptr);
        REQUIRE(u->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)u->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("postfix decrement") {
        TestContext test("4--");
        Node* n = nullptr;
        
        n = UnaryExpressionNode::TryParsePrefixUnaryExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::UnaryExpressionNode);
        REQUIRE(n->isError() == false);

        UnaryExpressionNode* u = (UnaryExpressionNode*)n;
        REQUIRE(u->operation == OperatorType::PostDecrement);
        
        REQUIRE(u->expr != nullptr);
        REQUIRE(u->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)u->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("prefix increment") {
        TestContext test("++4");
        Node* n = nullptr;
        
        n = UnaryExpressionNode::TryParsePrefixUnaryExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::UnaryExpressionNode);
        REQUIRE(n->isError() == false);

        UnaryExpressionNode* u = (UnaryExpressionNode*)n;
        REQUIRE(u->operation == OperatorType::PreIncrement);
        
        REQUIRE(u->expr != nullptr);
        REQUIRE(u->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)u->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("prefix decrement") {
        TestContext test("--4");
        Node* n = nullptr;
        
        n = UnaryExpressionNode::TryParsePrefixUnaryExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::UnaryExpressionNode);
        REQUIRE(n->isError() == false);

        UnaryExpressionNode* u = (UnaryExpressionNode*)n;
        REQUIRE(u->operation == OperatorType::PreDecrement);
        
        REQUIRE(u->expr != nullptr);
        REQUIRE(u->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)u->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("bitwise invert") {
        TestContext test("~4");
        Node* n = nullptr;
        
        n = UnaryExpressionNode::TryParsePrefixUnaryExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::UnaryExpressionNode);
        REQUIRE(n->isError() == false);

        UnaryExpressionNode* u = (UnaryExpressionNode*)n;
        REQUIRE(u->operation == OperatorType::BitwiseInvert);
        
        REQUIRE(u->expr != nullptr);
        REQUIRE(u->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)u->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("not") {
        TestContext test("!4");
        Node* n = nullptr;
        
        n = UnaryExpressionNode::TryParsePrefixUnaryExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::UnaryExpressionNode);
        REQUIRE(n->isError() == false);

        UnaryExpressionNode* u = (UnaryExpressionNode*)n;
        REQUIRE(u->operation == OperatorType::Not);
        
        REQUIRE(u->expr != nullptr);
        REQUIRE(u->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)u->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("negate") {
        TestContext test("-4");
        Node* n = nullptr;
        
        n = UnaryExpressionNode::TryParsePrefixUnaryExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::UnaryExpressionNode);
        REQUIRE(n->isError() == false);

        UnaryExpressionNode* u = (UnaryExpressionNode*)n;
        REQUIRE(u->operation == OperatorType::Negate);
        
        REQUIRE(u->expr != nullptr);
        REQUIRE(u->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)u->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("dereference") {
        TestContext test("*4");
        Node* n = nullptr;
        
        n = UnaryExpressionNode::TryParsePrefixUnaryExpression(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::UnaryExpressionNode);
        REQUIRE(n->isError() == false);

        UnaryExpressionNode* u = (UnaryExpressionNode*)n;
        REQUIRE(u->operation == OperatorType::Dereference);
        
        REQUIRE(u->expr != nullptr);
        REQUIRE(u->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)u->expr)->value.s == 4);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
}


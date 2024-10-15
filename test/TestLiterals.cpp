#include "Common.h"
#include <parse/literals/BooleanLiteralNode.h>
#include <parse/literals/NullLiteralNode.h>
#include <parse/literals/NumberLiteralNode.h>
#include <parse/literals/StringLiteralNode.h>
#include <parse/literals/TemplateStringLiteralNode.h>
#include <parse/proxies/ExpressionNode.h>

using namespace tokenize;
using namespace parse; 

TEST_CASE("Test Literal Nodes", "[parse]") {
    SECTION("Boolean literals") {
        TestContext test("true false notbool");
        BooleanLiteralNode* n1 = BooleanLiteralNode::TryParse(test.ctx);
        REQUIRE(n1 != nullptr);
        REQUIRE(n1->value == true);
        REQUIRE(n1->getType() == NodeType::BooleanLiteralNode);

        BooleanLiteralNode* n2 = BooleanLiteralNode::TryParse(test.ctx);
        REQUIRE(n2 != nullptr);
        REQUIRE(n2->value == false);
        REQUIRE(n1->getType() == NodeType::BooleanLiteralNode);

        BooleanLiteralNode* n3 = BooleanLiteralNode::TryParse(test.ctx);
        REQUIRE(n3 == nullptr);
    }

    SECTION("Null literal") {
        TestContext test("null");
        ExpressionNode* n1 = ExpressionNode::TryParsePrimaryExpression(test.ctx);
        REQUIRE(n1 != nullptr);
        REQUIRE(n1->getType() == NodeType::ExpressionNode);
        REQUIRE(n1->expr != nullptr);
        REQUIRE(n1->expr->getType() == NodeType::NullLiteralNode);
    }
    
    SECTION("Number literals") {
        TestContext test("1ub 2us 3ul 4ull 5b 6s 7l 8ll 9.0f 10.0d 11 12.0");
        NumberLiteralNode* n = nullptr;
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == false);
        REQUIRE(n->storageSize == 1);
        REQUIRE(n->value.u == 1);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == false);
        REQUIRE(n->storageSize == 2);
        REQUIRE(n->value.u == 2);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == false);
        REQUIRE(n->storageSize == 4);
        REQUIRE(n->value.u == 3);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == false);
        REQUIRE(n->storageSize == 8);
        REQUIRE(n->value.u == 4);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == true);
        REQUIRE(n->storageSize == 1);
        REQUIRE(n->value.u == 5);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == true);
        REQUIRE(n->storageSize == 2);
        REQUIRE(n->value.u == 6);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == true);
        REQUIRE(n->storageSize == 4);
        REQUIRE(n->value.s == 7);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == true);
        REQUIRE(n->storageSize == 8);
        REQUIRE(n->value.s == 8);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == true);
        REQUIRE(n->isSigned == true);
        REQUIRE(n->storageSize == 4);
        REQUIRE(n->value.f == 9.0f);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == true);
        REQUIRE(n->isSigned == true);
        REQUIRE(n->storageSize == 8);
        REQUIRE(n->value.d == 10.0);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == true);
        REQUIRE(n->storageSize == 4);
        REQUIRE(n->value.s == 11);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == true);
        REQUIRE(n->isSigned == true);
        REQUIRE(n->storageSize == 8);
        REQUIRE(n->value.d == 12.0);
    }

    SECTION("String literals") {
        TestContext test("'test \\'string\\'\n'");
        StringLiteralNode* n = nullptr;
        
        n = StringLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->getType() == NodeType::StringLiteralNode);
        REQUIRE((n->value == "test 'string'\n"));
    }
}
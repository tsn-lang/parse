#include "Common.h"
#include <parse/literals/BooleanLiteralNode.h>
#include <parse/literals/NullLiteralNode.h>
#include <parse/literals/NumberLiteralNode.h>
#include <parse/literals/StringLiteralNode.h>
#include <parse/literals/TemplateStringLiteralNode.h>
#include <parse/literals/ArrayLiteralNode.h>
#include <parse/literals/ObjectLiteralNode.h>
#include <parse/literals/ObjectLiteralPropertyNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/expressions/ExpressionSequenceNode.h>
#include <utils/Array.hpp>

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
        Node* n1 = ExpressionNode::TryParsePrimaryExpression(test.ctx);
        REQUIRE(n1 != nullptr);
        REQUIRE(n1->isError() == false);
        REQUIRE(n1->getType() == NodeType::NullLiteralNode);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("Number literals") {
        TestContext test("1ub 2us 3ul 4ull 5b 6s 7l 8ll 9.0f 10.0d 11 12.0");
        NumberLiteralNode* n = nullptr;
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == false);
        REQUIRE(n->storageSize == 1);
        REQUIRE(n->value.u == 1);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == false);
        REQUIRE(n->storageSize == 2);
        REQUIRE(n->value.u == 2);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == false);
        REQUIRE(n->storageSize == 4);
        REQUIRE(n->value.u == 3);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == false);
        REQUIRE(n->storageSize == 8);
        REQUIRE(n->value.u == 4);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == true);
        REQUIRE(n->storageSize == 1);
        REQUIRE(n->value.u == 5);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == true);
        REQUIRE(n->storageSize == 2);
        REQUIRE(n->value.u == 6);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == true);
        REQUIRE(n->storageSize == 4);
        REQUIRE(n->value.s == 7);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
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
        REQUIRE(n->isError() == false);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == true);
        REQUIRE(n->isSigned == true);
        REQUIRE(n->storageSize == 8);
        REQUIRE(n->value.d == 10.0);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == false);
        REQUIRE(n->isSigned == true);
        REQUIRE(n->storageSize == 4);
        REQUIRE(n->value.s == 11);
        
        n = NumberLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->getType() == NodeType::NumberLiteralNode);
        REQUIRE(n->isFloat == true);
        REQUIRE(n->isSigned == true);
        REQUIRE(n->storageSize == 8);
        REQUIRE(n->value.d == 12.0);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("String literals") {
        TestContext test("'test \\'string\\'\n'");
        StringLiteralNode* n = nullptr;
        
        n = StringLiteralNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->getType() == NodeType::StringLiteralNode);
        REQUIRE(n->value == "test 'string'\n");
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("Template string literals") {
        TestContext test("`this ${'is'} a ${`test`}. ${123}`");
        //                0123456789111111111122222222223333
        //                          012345678901234567890123
        TemplateStringLiteralNode* n = nullptr;

        n = TemplateStringLiteralNode::TryParse(test.ctx);
        SourceLocation loc;

        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->segments.size() == 6);

        REQUIRE(n->segments[0].expr == nullptr);
        REQUIRE(n->segments[0].text == "this ");

        REQUIRE(n->segments[1].expr != nullptr);
        REQUIRE(n->segments[1].text.size() == 0);
        REQUIRE(n->segments[1].expr->expr != nullptr);
        REQUIRE(n->segments[1].expr->expr->getType() == NodeType::StringLiteralNode);
        StringLiteralNode* sl = (StringLiteralNode*)n->segments[1].expr->expr;
        REQUIRE(sl->value == "is");
        loc = n->segments[1].expr->expr->getLocation();
        REQUIRE(loc.startBufferPosition == 8);
        REQUIRE(loc.endBufferPosition == 12);

        REQUIRE(n->segments[2].expr == nullptr);
        REQUIRE(n->segments[2].text == " a ");

        REQUIRE(n->segments[3].expr != nullptr);
        REQUIRE(n->segments[3].text.size() == 0);
        REQUIRE(n->segments[3].expr->expr != nullptr);
        REQUIRE(n->segments[3].expr->expr->getType() == NodeType::TemplateStringLiteralNode);
        TemplateStringLiteralNode* tl = (TemplateStringLiteralNode*)n->segments[3].expr->expr;
        REQUIRE(tl->segments.size() == 1);
        REQUIRE(tl->segments[0].expr == nullptr);
        REQUIRE(tl->segments[0].text == "test");
        REQUIRE(tl->getLocation().startBufferPosition == 18);
        REQUIRE(tl->getLocation().endBufferPosition == 24);

        REQUIRE(n->segments[4].expr == nullptr);
        REQUIRE(n->segments[4].text == ". ");

        REQUIRE(n->segments[5].expr != nullptr);
        REQUIRE(n->segments[5].text.size() == 0);
        REQUIRE(n->segments[5].expr->expr != nullptr);
        REQUIRE(n->segments[5].expr->expr->getType() == NodeType::NumberLiteralNode);
        NumberLiteralNode* nl = (NumberLiteralNode*)n->segments[5].expr->expr;
        REQUIRE(nl->value.s == 123);
        REQUIRE(nl->getLocation().startBufferPosition == 29);
        REQUIRE(nl->getLocation().endBufferPosition == 32);

        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("Array literals") {
        TestContext test0("[]");
        ArrayLiteralNode* n = nullptr;
        
        n = ArrayLiteralNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->elements == nullptr);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);
        
        TestContext test1("[1, 2]");
        n = ArrayLiteralNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->elements != nullptr);
        REQUIRE(n->elements->expressions.size() == 2);

        REQUIRE(n->elements->expressions[0] != nullptr);
        REQUIRE(n->elements->expressions[0]->expr != nullptr);
        REQUIRE(n->elements->expressions[0]->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)n->elements->expressions[0]->expr)->value.s == 1);

        REQUIRE(n->elements->expressions[1] != nullptr);
        REQUIRE(n->elements->expressions[1]->expr != nullptr);
        REQUIRE(n->elements->expressions[1]->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)n->elements->expressions[1]->expr)->value.s == 2);

        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }

    SECTION("Object literals") {
        TestContext test0("{}");
        ObjectLiteralNode* n = nullptr;
        
        n = ObjectLiteralNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->properties.size() == 0);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);
        
        TestContext test1("{ a: 1, b: 2 }");
        n = ObjectLiteralNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(n->properties.size() == 2);
        
        REQUIRE(n->properties[0] != nullptr);
        REQUIRE(n->properties[0]->name == "a");
        REQUIRE(n->properties[0]->value != nullptr);
        REQUIRE(n->properties[0]->value->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)n->properties[0]->value)->expr != nullptr);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)n->properties[0]->value)->expr)->value.s == 1);

        REQUIRE(n->properties[1] != nullptr);
        REQUIRE(n->properties[1]->name == "b");
        REQUIRE(n->properties[1]->value != nullptr);
        REQUIRE(n->properties[1]->value->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)n->properties[1]->value)->expr != nullptr);
        REQUIRE(((NumberLiteralNode*)((ExpressionNode*)n->properties[1]->value)->expr)->value.s == 2);

        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
}
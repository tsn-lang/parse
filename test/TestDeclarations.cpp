#include "Common.h"
// ClassNode tested by TestClassNodes.cpp
#include <parse/declarations/EnumNode.h>
#include <parse/declarations/EnumFieldNode.h>
#include <parse/declarations/FunctionNode.h>
#include <parse/declarations/TypeNode.h>
#include <parse/literals/NumberLiteralNode.h>
#include <parse/misc/ParameterListNode.h>
#include <parse/misc/TypeParameterListNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/types/IdentifierTypeSpecifierNode.h>
#include <utils/Array.hpp>

using namespace tokenize;
using namespace parse;

TEST_CASE("Test EnumFieldNode", "[parse]") {
    SECTION("name") {
        TestContext test("field = 0");
        EnumFieldNode* n = EnumFieldNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->name == "field");
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("value") {
        TestContext test0("field = 0");
        EnumFieldNode* n = nullptr;
        
        n = EnumFieldNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->value != nullptr);
        REQUIRE(n->value->isSigned == true);
        REQUIRE(n->value->storageSize == 4);
        REQUIRE(n->value->value.s == 0);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);
        
        TestContext test1("field = 1ull");
        n = EnumFieldNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->value != nullptr);
        REQUIRE(n->value->isSigned == false);
        REQUIRE(n->value->storageSize == 8);
        REQUIRE(n->value->value.s == 1);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);
    }
}

TEST_CASE("Test EnumNode", "[parse]") {
    SECTION("declaration") {
        TestContext test("enum Test;");
        EnumNode* n = EnumNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->name == "Test");
        REQUIRE(n->fields.size() == 0);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("definition") {
        TestContext test(
            "enum Test {\n"
            "    field0 = 0,\n"
            "    field1 = 1\n"
            "}\n"
        );
        EnumNode* n = EnumNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->name == "Test");
        REQUIRE(n->fields.size() == 2);
        REQUIRE(n->fields[0] != nullptr);
        REQUIRE(n->fields[0]->isError() == false);
        REQUIRE(n->fields[0]->value != nullptr);
        REQUIRE(n->fields[0]->value->isSigned == true);
        REQUIRE(n->fields[0]->value->storageSize == 4);
        REQUIRE(n->fields[0]->value->value.s == 0);
        REQUIRE(n->fields[1] != nullptr);
        REQUIRE(n->fields[1]->isError() == false);
        REQUIRE(n->fields[1]->value != nullptr);
        REQUIRE(n->fields[1]->value->isSigned == true);
        REQUIRE(n->fields[1]->value->storageSize == 4);
        REQUIRE(n->fields[1]->value->value.s == 1);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
}

TEST_CASE("Test FunctionNode", "[parse]") {
    SECTION("name") {
        TestContext test("function test() : void;");
        FunctionNode* n = FunctionNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->name == "test");
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("async") {
        TestContext test("async function test() : void;");
        FunctionNode* n = FunctionNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isAsync == true);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("parameters") {
        FunctionNode* n = nullptr;
        
        TestContext test0("function test() : void;");
        n = FunctionNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 0);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("function test(p1: i32) : void;");
        n = FunctionNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 1);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("type parameters") {
        FunctionNode* n = nullptr;
        
        TestContext test0("function test() : void;");
        n = FunctionNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->typeParameters == nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("function test<P1>() : void;");
        n = FunctionNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->typeParameters != nullptr);
        REQUIRE(n->typeParameters->parameters.size() == 1);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("body") {
        FunctionNode* n = nullptr;
        
        TestContext test0("function test() : void;");
        n = FunctionNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->body == nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("function test() : void {}");
        n = FunctionNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->body != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }

    SECTION("return type") {
        FunctionNode* n = nullptr;
        
        TestContext test0("function test() : void;");
        n = FunctionNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->returnType != nullptr);
        REQUIRE(n->returnType->type != nullptr);
        REQUIRE(n->returnType->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->returnType->type)->name == "void");
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("function test() : i32;");
        n = FunctionNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->returnType != nullptr);
        REQUIRE(n->returnType->type != nullptr);
        REQUIRE(n->returnType->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->returnType->type)->name == "i32");
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
}

TEST_CASE("Test TypeNode", "[parse]") {
    SECTION("name") {
        TestContext test("type Test;");
        TypeNode* n = TypeNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->name == "Test");
        REQUIRE(n->type == nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("type parameters") {
        TypeNode* n = nullptr;
        
        TestContext test0("type Test;");
        n = TypeNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters == nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("type Test<P1>;");
        n = TypeNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 1);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("body") {
        TestContext test("type Test = void;");

        TypeNode* n = TypeNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->type != nullptr);
        REQUIRE(n->type->type != nullptr);
        REQUIRE(n->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->type->type)->name == "void");
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
}
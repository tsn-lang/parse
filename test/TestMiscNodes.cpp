#include "Common.h"
#include <parse/misc/IdentifierNode.h>
#include <parse/misc/ModuleNode.h>
#include <parse/misc/ParameterListNode.h>
#include <parse/misc/TypeParameterNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/misc/TypeParameterListNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/types/IdentifierTypeSpecifierNode.h>
#include <utils/Array.hpp>

using namespace tokenize;
using namespace parse; 

TEST_CASE("Test IdentifierNode", "[parse]") {
    TestContext test("test");
    IdentifierNode* n = IdentifierNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->text == "test");
    REQUIRE(n->isError() == false);
    REQUIRE(test.ctx->getState()->messages.size() == 0);
}

TEST_CASE("Test ModuleNode", "[parse]") {
    TestContext test(
        "function testFuncDecl() : void;\n"
        "function testFuncDef() : void {}\n"
        "class TestClassDecl;\n"
        "class TestClassDef {}\n"
        "enum TestEnumDecl;\n"
        "enum TestEnumDef {}\n"
        "import * from 'module';\n"
        "export testFuncDef;\n"
        "const testStatement = 5;\n"
    );
    ModuleNode* n = ModuleNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->statements.size() == 9);
    REQUIRE(n->isError() == false);
    REQUIRE(test.ctx->getState()->messages.size() == 0);
}

TEST_CASE("Test ParameterListNode", "[parse]") {
    SECTION("no parameters") {
        TestContext test("()");
        ParameterListNode* n = ParameterListNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters.size() == 0);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("one parameter") {
        TestContext test("(p1: i32)");
        ParameterListNode* n = ParameterListNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters.size() == 1);
        REQUIRE(n->parameters[0] != nullptr);
        REQUIRE(n->parameters[0]->name != nullptr);
        REQUIRE(n->parameters[0]->name->text == "p1");
        REQUIRE(n->parameters[0]->type != nullptr);
        REQUIRE(n->parameters[0]->type->type != nullptr);
        REQUIRE(n->parameters[0]->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->parameters[0]->type->type)->name != nullptr);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->parameters[0]->type->type)->name->text == "i32");
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("multiple parameters") {
        TestContext test("(p1: i32, p2: i64, p3: void)");
        ParameterListNode* n = ParameterListNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters.size() == 3);
        REQUIRE(n->parameters[0] != nullptr);
        REQUIRE(n->parameters[0]->name != nullptr);
        REQUIRE(n->parameters[0]->name->text == "p1");
        REQUIRE(n->parameters[0]->type != nullptr);
        REQUIRE(n->parameters[0]->type->type != nullptr);
        REQUIRE(n->parameters[0]->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->parameters[0]->type->type)->name != nullptr);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->parameters[0]->type->type)->name->text == "i32");
        REQUIRE(n->parameters[1] != nullptr);
        REQUIRE(n->parameters[1]->name != nullptr);
        REQUIRE(n->parameters[1]->name->text == "p2");
        REQUIRE(n->parameters[1]->type != nullptr);
        REQUIRE(n->parameters[1]->type->type != nullptr);
        REQUIRE(n->parameters[1]->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->parameters[1]->type->type)->name != nullptr);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->parameters[1]->type->type)->name->text == "i64");
        REQUIRE(n->parameters[2] != nullptr);
        REQUIRE(n->parameters[2]->name != nullptr);
        REQUIRE(n->parameters[2]->name->text == "p3");
        REQUIRE(n->parameters[2]->type != nullptr);
        REQUIRE(n->parameters[2]->type->type != nullptr);
        REQUIRE(n->parameters[2]->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->parameters[2]->type->type)->name != nullptr);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->parameters[2]->type->type)->name->text == "void");
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
}

TEST_CASE("Test TypedAssignableNode", "[parse]") {
    TestContext test("p1: i32");
    TypedAssignableNode* n = TypedAssignableNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->name != nullptr);
    REQUIRE(n->name->text == "p1");
    REQUIRE(n->type != nullptr);
    REQUIRE(n->type->type != nullptr);
    REQUIRE(n->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
    REQUIRE(((IdentifierTypeSpecifierNode*)n->type->type)->name != nullptr);
    REQUIRE(((IdentifierTypeSpecifierNode*)n->type->type)->name->text == "i32");
    REQUIRE(n->isError() == false);
    REQUIRE(test.ctx->getState()->messages.size() == 0);
}

TEST_CASE("Test TypeParameterListNode", "[parse]") {
    SECTION("no parameters") {
        TestContext test("<>");
        TypeParameterListNode* n = TypeParameterListNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters.size() == 0);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("one parameter") {
        TestContext test("<P1>");
        TypeParameterListNode* n = TypeParameterListNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters.size() == 1);
        REQUIRE(n->parameters[0] != nullptr);
        REQUIRE(n->parameters[0]->name == "P1");
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("multiple parameters") {
        TestContext test("<P1, P2, P3>");
        TypeParameterListNode* n = TypeParameterListNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters.size() == 3);
        REQUIRE(n->parameters[0] != nullptr);
        REQUIRE(n->parameters[0]->name == "P1");
        REQUIRE(n->parameters[1] != nullptr);
        REQUIRE(n->parameters[1]->name == "P2");
        REQUIRE(n->parameters[2] != nullptr);
        REQUIRE(n->parameters[2]->name == "P3");
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    // todo: type constraints
}

TEST_CASE("Test TypeParameterNode", "[parse]") {
    TestContext test("Test");
    TypeParameterNode* n = TypeParameterNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->name == "Test");
    REQUIRE(n->isError() == false);
    REQUIRE(test.ctx->getState()->messages.size() == 0);
}

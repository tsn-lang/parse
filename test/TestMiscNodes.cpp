#include "Common.h"
#include <parse/misc/IdentifierNode.h>
#include <parse/misc/ModuleNode.h>
#include <utils/Array.hpp>

using namespace tokenize;
using namespace parse; 

TEST_CASE("IdentifierNode", "[parse]") {
    TestContext test("test");
    IdentifierNode* n = IdentifierNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->text == "test");
    REQUIRE(n->isError() == false);
    REQUIRE(test.ctx->getState()->messages.size() == 0);
}

TEST_CASE("ModuleNode", "[parse]") {
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
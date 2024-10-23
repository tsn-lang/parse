#include "Common.h"
#include <parse/statements/CatchStatementNode.h>
#include <parse/statements/DeclarationStatementNode.h>
#include <parse/statements/DeleteStatementNode.h>
#include <parse/statements/DoWhileLoopNode.h>
#include <parse/statements/ExportStatementNode.h>
#include <parse/statements/ExpressionStatementNode.h>
#include <parse/statements/ForLoopNode.h>
#include <parse/statements/IfStatementNode.h>
#include <parse/statements/ImportAllStatementNode.h>
#include <parse/statements/ImportSelectStatementNode.h>
#include <parse/statements/ReturnStatementNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <parse/statements/SwitchCaseNode.h>
#include <parse/statements/SwitchStatementNode.h>
#include <parse/statements/ThrowStatementNode.h>
#include <parse/statements/TryCatchStatementNode.h>
#include <parse/statements/WhileLoopNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/misc/IdentifierNode.h>
#include <parse/types/IdentifierTypeSpecifierNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/proxies/StatementNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/literals/NumberLiteralNode.h>
#include <utils/Array.hpp>

using namespace tokenize;
using namespace parse; 

TEST_CASE("Test CatchStatementNode") {
    TestContext test("catch(exc: i32) {}");
    CatchStatementNode* n = CatchStatementNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->isError() == false);
    REQUIRE(test.ctx->getState()->messages.size() == 0);
    REQUIRE(n->exception != nullptr);
    REQUIRE(n->exception->name == "exc");
    REQUIRE(n->exception->type != nullptr);
    REQUIRE(n->exception->type->type != nullptr);
    REQUIRE(n->exception->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
    REQUIRE(((IdentifierTypeSpecifierNode*)n->exception->type->type)->name == "i32");
    REQUIRE(n->body != nullptr);
    REQUIRE(n->body->stmt != nullptr);
    REQUIRE(n->body->stmt->getType() == NodeType::StatementBlockNode);
}

TEST_CASE("Test DeclarationStatementNode") {
    SECTION("uninitialized const") {
        TestContext test("const var;");
        DeclarationStatementNode* n = DeclarationStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->assignable != nullptr);
        REQUIRE(n->assignable->text == "var");
        REQUIRE(n->typedAssignable == nullptr);
        REQUIRE(n->isConst == true);
        REQUIRE(n->isArrayDestructure == false);
        REQUIRE(n->isObjectDestructure == false);
        REQUIRE(n->destructureTargets.size() == 0);
        REQUIRE(n->initializer == nullptr);
    }

    SECTION("uninitialized typed const") {
        TestContext test("const var : i32;");
        DeclarationStatementNode* n = DeclarationStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->assignable == nullptr);
        REQUIRE(n->typedAssignable != nullptr);
        REQUIRE(n->typedAssignable->type != nullptr);
        REQUIRE(n->typedAssignable->type->type != nullptr);
        REQUIRE(n->typedAssignable->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->typedAssignable->type->type)->name == "i32");
        REQUIRE(n->typedAssignable->name == "var");
        REQUIRE(n->isConst == true);
        REQUIRE(n->isArrayDestructure == false);
        REQUIRE(n->isObjectDestructure == false);
        REQUIRE(n->destructureTargets.size() == 0);
        REQUIRE(n->initializer == nullptr);
    }
    
    SECTION("uninitialized const object destructure") {
        TestContext test("const { a, b, c };");
        DeclarationStatementNode* n = DeclarationStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->assignable == nullptr);
        REQUIRE(n->typedAssignable == nullptr);
        REQUIRE(n->isConst == true);
        REQUIRE(n->isArrayDestructure == false);
        REQUIRE(n->isObjectDestructure == true);
        REQUIRE(n->destructureTargets.size() == 3);
        REQUIRE(n->destructureTargets[0] != nullptr);
        REQUIRE(n->destructureTargets[0]->text == "a");
        REQUIRE(n->destructureTargets[1] != nullptr);
        REQUIRE(n->destructureTargets[1]->text == "b");
        REQUIRE(n->destructureTargets[2] != nullptr);
        REQUIRE(n->destructureTargets[2]->text == "c");
        REQUIRE(n->initializer == nullptr);
    }

    SECTION("uninitialized const array destructure") {
        TestContext test("const [ a, b, c ];");
        DeclarationStatementNode* n = DeclarationStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->assignable == nullptr);
        REQUIRE(n->typedAssignable == nullptr);
        REQUIRE(n->isConst == true);
        REQUIRE(n->isArrayDestructure == true);
        REQUIRE(n->isObjectDestructure == false);
        REQUIRE(n->destructureTargets.size() == 3);
        REQUIRE(n->destructureTargets[0] != nullptr);
        REQUIRE(n->destructureTargets[0]->text == "a");
        REQUIRE(n->destructureTargets[1] != nullptr);
        REQUIRE(n->destructureTargets[1]->text == "b");
        REQUIRE(n->destructureTargets[2] != nullptr);
        REQUIRE(n->destructureTargets[2]->text == "c");
        REQUIRE(n->initializer == nullptr);
    }

    SECTION("initialized const") {
        TestContext test("const var = 1;");
        DeclarationStatementNode* n = DeclarationStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->assignable != nullptr);
        REQUIRE(n->assignable->text == "var");
        REQUIRE(n->typedAssignable == nullptr);
        REQUIRE(n->isConst == true);
        REQUIRE(n->isArrayDestructure == false);
        REQUIRE(n->isObjectDestructure == false);
        REQUIRE(n->destructureTargets.size() == 0);
        REQUIRE(n->initializer != nullptr);
        REQUIRE(n->initializer->expr != nullptr);
        REQUIRE(n->initializer->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)n->initializer->expr)->value.s == 1);
    }

    SECTION("initialized typed const") {
        TestContext test("const var : i32 = 1;");
        DeclarationStatementNode* n = DeclarationStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->assignable == nullptr);
        REQUIRE(n->typedAssignable != nullptr);
        REQUIRE(n->typedAssignable->type != nullptr);
        REQUIRE(n->typedAssignable->type->type != nullptr);
        REQUIRE(n->typedAssignable->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->typedAssignable->type->type)->name == "i32");
        REQUIRE(n->typedAssignable->name == "var");
        REQUIRE(n->isConst == true);
        REQUIRE(n->isArrayDestructure == false);
        REQUIRE(n->isObjectDestructure == false);
        REQUIRE(n->destructureTargets.size() == 0);
        REQUIRE(n->initializer != nullptr);
        REQUIRE(n->initializer->expr != nullptr);
        REQUIRE(n->initializer->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)n->initializer->expr)->value.s == 1);
    }

    SECTION("initialized const object destructure") {
        TestContext test("const { a, b, c } = var;");
        DeclarationStatementNode* n = DeclarationStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->assignable == nullptr);
        REQUIRE(n->typedAssignable == nullptr);
        REQUIRE(n->isConst == true);
        REQUIRE(n->isArrayDestructure == false);
        REQUIRE(n->isObjectDestructure == true);
        REQUIRE(n->destructureTargets.size() == 3);
        REQUIRE(n->destructureTargets[0] != nullptr);
        REQUIRE(n->destructureTargets[0]->text == "a");
        REQUIRE(n->destructureTargets[1] != nullptr);
        REQUIRE(n->destructureTargets[1]->text == "b");
        REQUIRE(n->destructureTargets[2] != nullptr);
        REQUIRE(n->destructureTargets[2]->text == "c");
        REQUIRE(n->initializer != nullptr);
        REQUIRE(n->initializer->expr != nullptr);
        REQUIRE(n->initializer->expr->getType() == NodeType::IdentifierNode);
        REQUIRE(((IdentifierNode*)n->initializer->expr)->text == "var");
    }

    SECTION("initialized const array destructure") {
        TestContext test("const [ a, b, c ] = var;");
        DeclarationStatementNode* n = DeclarationStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->assignable == nullptr);
        REQUIRE(n->typedAssignable == nullptr);
        REQUIRE(n->isConst == true);
        REQUIRE(n->isArrayDestructure == true);
        REQUIRE(n->isObjectDestructure == false);
        REQUIRE(n->destructureTargets.size() == 3);
        REQUIRE(n->destructureTargets[0] != nullptr);
        REQUIRE(n->destructureTargets[0]->text == "a");
        REQUIRE(n->destructureTargets[1] != nullptr);
        REQUIRE(n->destructureTargets[1]->text == "b");
        REQUIRE(n->destructureTargets[2] != nullptr);
        REQUIRE(n->destructureTargets[2]->text == "c");
        REQUIRE(n->initializer != nullptr);
        REQUIRE(n->initializer->expr != nullptr);
        REQUIRE(n->initializer->expr->getType() == NodeType::IdentifierNode);
        REQUIRE(((IdentifierNode*)n->initializer->expr)->text == "var");
    }

    SECTION("uninitialized let") {
        TestContext test("let var;");
        DeclarationStatementNode* n = DeclarationStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->assignable != nullptr);
        REQUIRE(n->assignable->text == "var");
        REQUIRE(n->typedAssignable == nullptr);
        REQUIRE(n->isConst == false);
        REQUIRE(n->isArrayDestructure == false);
        REQUIRE(n->isObjectDestructure == false);
        REQUIRE(n->destructureTargets.size() == 0);
        REQUIRE(n->initializer == nullptr);
    }

    SECTION("uninitialized typed let") {
        TestContext test("let var : i32;");
        DeclarationStatementNode* n = DeclarationStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->assignable == nullptr);
        REQUIRE(n->typedAssignable != nullptr);
        REQUIRE(n->typedAssignable->type != nullptr);
        REQUIRE(n->typedAssignable->type->type != nullptr);
        REQUIRE(n->typedAssignable->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->typedAssignable->type->type)->name == "i32");
        REQUIRE(n->typedAssignable->name == "var");
        REQUIRE(n->isConst == false);
        REQUIRE(n->isArrayDestructure == false);
        REQUIRE(n->isObjectDestructure == false);
        REQUIRE(n->destructureTargets.size() == 0);
        REQUIRE(n->initializer == nullptr);
    }
    
    SECTION("uninitialized let object destructure") {
        TestContext test("let { a, b, c };");
        DeclarationStatementNode* n = DeclarationStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->assignable == nullptr);
        REQUIRE(n->typedAssignable == nullptr);
        REQUIRE(n->isConst == false);
        REQUIRE(n->isArrayDestructure == false);
        REQUIRE(n->isObjectDestructure == true);
        REQUIRE(n->destructureTargets.size() == 3);
        REQUIRE(n->destructureTargets[0] != nullptr);
        REQUIRE(n->destructureTargets[0]->text == "a");
        REQUIRE(n->destructureTargets[1] != nullptr);
        REQUIRE(n->destructureTargets[1]->text == "b");
        REQUIRE(n->destructureTargets[2] != nullptr);
        REQUIRE(n->destructureTargets[2]->text == "c");
        REQUIRE(n->initializer == nullptr);
    }

    SECTION("uninitialized let array destructure") {
        TestContext test("let [ a, b, c ];");
        DeclarationStatementNode* n = DeclarationStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->assignable == nullptr);
        REQUIRE(n->typedAssignable == nullptr);
        REQUIRE(n->isConst == false);
        REQUIRE(n->isArrayDestructure == true);
        REQUIRE(n->isObjectDestructure == false);
        REQUIRE(n->destructureTargets.size() == 3);
        REQUIRE(n->destructureTargets[0] != nullptr);
        REQUIRE(n->destructureTargets[0]->text == "a");
        REQUIRE(n->destructureTargets[1] != nullptr);
        REQUIRE(n->destructureTargets[1]->text == "b");
        REQUIRE(n->destructureTargets[2] != nullptr);
        REQUIRE(n->destructureTargets[2]->text == "c");
        REQUIRE(n->initializer == nullptr);
    }

    SECTION("initialized let") {
        TestContext test("let var = 1;");
        DeclarationStatementNode* n = DeclarationStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->assignable != nullptr);
        REQUIRE(n->assignable->text == "var");
        REQUIRE(n->typedAssignable == nullptr);
        REQUIRE(n->isConst == false);
        REQUIRE(n->isArrayDestructure == false);
        REQUIRE(n->isObjectDestructure == false);
        REQUIRE(n->destructureTargets.size() == 0);
        REQUIRE(n->initializer != nullptr);
        REQUIRE(n->initializer->expr != nullptr);
        REQUIRE(n->initializer->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)n->initializer->expr)->value.s == 1);
    }

    SECTION("initialized typed let") {
        TestContext test("let var : i32 = 1;");
        DeclarationStatementNode* n = DeclarationStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->assignable == nullptr);
        REQUIRE(n->typedAssignable != nullptr);
        REQUIRE(n->typedAssignable->type != nullptr);
        REQUIRE(n->typedAssignable->type->type != nullptr);
        REQUIRE(n->typedAssignable->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->typedAssignable->type->type)->name == "i32");
        REQUIRE(n->typedAssignable->name == "var");
        REQUIRE(n->isConst == false);
        REQUIRE(n->isArrayDestructure == false);
        REQUIRE(n->isObjectDestructure == false);
        REQUIRE(n->destructureTargets.size() == 0);
        REQUIRE(n->initializer != nullptr);
        REQUIRE(n->initializer->expr != nullptr);
        REQUIRE(n->initializer->expr->getType() == NodeType::NumberLiteralNode);
        REQUIRE(((NumberLiteralNode*)n->initializer->expr)->value.s == 1);
    }

    SECTION("initialized let object destructure") {
        TestContext test("let { a, b, c } = var;");
        DeclarationStatementNode* n = DeclarationStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->assignable == nullptr);
        REQUIRE(n->typedAssignable == nullptr);
        REQUIRE(n->isConst == false);
        REQUIRE(n->isArrayDestructure == false);
        REQUIRE(n->isObjectDestructure == true);
        REQUIRE(n->destructureTargets.size() == 3);
        REQUIRE(n->destructureTargets[0] != nullptr);
        REQUIRE(n->destructureTargets[0]->text == "a");
        REQUIRE(n->destructureTargets[1] != nullptr);
        REQUIRE(n->destructureTargets[1]->text == "b");
        REQUIRE(n->destructureTargets[2] != nullptr);
        REQUIRE(n->destructureTargets[2]->text == "c");
        REQUIRE(n->initializer != nullptr);
        REQUIRE(n->initializer->expr != nullptr);
        REQUIRE(n->initializer->expr->getType() == NodeType::IdentifierNode);
        REQUIRE(((IdentifierNode*)n->initializer->expr)->text == "var");
    }

    SECTION("initialized let array destructure") {
        TestContext test("let [ a, b, c ] = var;");
        DeclarationStatementNode* n = DeclarationStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->assignable == nullptr);
        REQUIRE(n->typedAssignable == nullptr);
        REQUIRE(n->isConst == false);
        REQUIRE(n->isArrayDestructure == true);
        REQUIRE(n->isObjectDestructure == false);
        REQUIRE(n->destructureTargets.size() == 3);
        REQUIRE(n->destructureTargets[0] != nullptr);
        REQUIRE(n->destructureTargets[0]->text == "a");
        REQUIRE(n->destructureTargets[1] != nullptr);
        REQUIRE(n->destructureTargets[1]->text == "b");
        REQUIRE(n->destructureTargets[2] != nullptr);
        REQUIRE(n->destructureTargets[2]->text == "c");
        REQUIRE(n->initializer != nullptr);
        REQUIRE(n->initializer->expr != nullptr);
        REQUIRE(n->initializer->expr->getType() == NodeType::IdentifierNode);
        REQUIRE(((IdentifierNode*)n->initializer->expr)->text == "var");
    }
}

TEST_CASE("Test DeleteStatementNode") {
    TestContext test("delete var");
    DeleteStatementNode* n = DeleteStatementNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->isError() == false);
    REQUIRE(test.ctx->getState()->messages.size() == 0);
    REQUIRE(n->expr != nullptr);
    REQUIRE(n->expr->getType() == NodeType::ExpressionNode);
    REQUIRE(((ExpressionNode*)n->expr)->expr != nullptr);
    REQUIRE(((ExpressionNode*)n->expr)->expr->getType() == NodeType::IdentifierNode);
    REQUIRE(((IdentifierNode*)((ExpressionNode*)n->expr)->expr)->text == "var");
}

TEST_CASE("Test DoWhileLoopNode") {
    TestContext test("do test(); while (cond)");
    DoWhileLoopNode* n = DoWhileLoopNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->isError() == false);
    REQUIRE(test.ctx->getState()->messages.size() == 0);
    REQUIRE(n->condition != nullptr);
    REQUIRE(n->condition->getType() == NodeType::ExpressionNode);
    REQUIRE(((ExpressionNode*)n->condition)->expr != nullptr);
    REQUIRE(((ExpressionNode*)n->condition)->expr->getType() == NodeType::IdentifierNode);
    REQUIRE(((IdentifierNode*)((ExpressionNode*)n->condition)->expr)->text == "cond");
    REQUIRE(n->body != nullptr);
    REQUIRE(n->body->stmt != nullptr);
    REQUIRE(n->body->stmt->getType() == NodeType::ExpressionStatementNode);
}

TEST_CASE("Test ExportStatementNode") {
    SECTION("Export class") {
        TestContext test("export class Test");
        ExportStatementNode* n = ExportStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->exportable != nullptr);
        REQUIRE(n->exportable->getType() == NodeType::ClassNode);
    }

    SECTION("Export function") {
        TestContext test("export function func() : i32");
        ExportStatementNode* n = ExportStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->exportable != nullptr);
        REQUIRE(n->exportable->getType() == NodeType::FunctionNode);
    }

    SECTION("Export type") {
        TestContext test("export type Test");
        ExportStatementNode* n = ExportStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->exportable != nullptr);
        REQUIRE(n->exportable->getType() == NodeType::TypeNode);
    }

    SECTION("Export enum") {
        TestContext test("export enum Test");
        ExportStatementNode* n = ExportStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->exportable != nullptr);
        REQUIRE(n->exportable->getType() == NodeType::EnumNode);
    }

    SECTION("Export variable decl") {
        TestContext test("export const test");
        ExportStatementNode* n = ExportStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->exportable != nullptr);
        REQUIRE(n->exportable->getType() == NodeType::DeclarationStatementNode);
    }

    SECTION("Export identifier") {
        TestContext test("export test");
        ExportStatementNode* n = ExportStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->exportable != nullptr);
        REQUIRE(n->exportable->getType() == NodeType::IdentifierNode);
    }
}

TEST_CASE("Test ExpressionStatementNode") {
    SECTION("single expression") {
        TestContext test("1 + 1");
        ExpressionStatementNode* n = ExpressionStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->expressions.size() == 1);
        REQUIRE(n->expressions[0] != nullptr);
        REQUIRE(n->expressions[0]->expr != nullptr);
        REQUIRE(n->expressions[0]->expr->getType() == NodeType::BinaryExpressionNode);
    }

    SECTION("multiple expressions") {
        TestContext test("1 + 1, 1 + 2");
        ExpressionStatementNode* n = ExpressionStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->expressions.size() == 2);
        REQUIRE(n->expressions[0] != nullptr);
        REQUIRE(n->expressions[0]->expr != nullptr);
        REQUIRE(n->expressions[0]->expr->getType() == NodeType::BinaryExpressionNode);
        REQUIRE(n->expressions[1] != nullptr);
        REQUIRE(n->expressions[1]->expr != nullptr);
        REQUIRE(n->expressions[1]->expr->getType() == NodeType::BinaryExpressionNode);
    }
}

TEST_CASE("Test ForLoopNode") {
    TestContext test("for (let i = 0; i < 10;i++) { }");
    ForLoopNode* n = ForLoopNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->isError() == false);
    REQUIRE(test.ctx->getState()->messages.size() == 0);

    REQUIRE(n->initializer != nullptr);

    REQUIRE(n->condition != nullptr);
    REQUIRE(((ExpressionNode*)n->condition)->expr != nullptr);
    REQUIRE(((ExpressionNode*)n->condition)->expr->getType() == NodeType::BinaryExpressionNode);

    REQUIRE(n->body != nullptr);
    REQUIRE(n->body->stmt != nullptr);
    REQUIRE(n->body->stmt->getType() == NodeType::StatementBlockNode);
}

TEST_CASE("Test IfStatementNode") {
    TestContext test("if (cond) { }");
    IfStatementNode* n = IfStatementNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->isError() == false);
    REQUIRE(test.ctx->getState()->messages.size() == 0);

    REQUIRE(n->condition != nullptr);
    REQUIRE(n->condition->expr != nullptr);
    REQUIRE(n->condition->expr->getType() == NodeType::IdentifierNode);
    REQUIRE(((IdentifierNode*)n->condition->expr)->text == "cond");

    REQUIRE(n->body != nullptr);
    REQUIRE(n->body->stmt != nullptr);
    REQUIRE(n->body->stmt->getType() == NodeType::StatementBlockNode);
}

TEST_CASE("Test ImportAllStatementNode") {
    SECTION("unaliased") {
        TestContext test("import * from 'module'");
        ImportAllStatementNode* n = ImportAllStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->moduleId == "module");
        REQUIRE(n->alias.size() == 0);
    }
    
    SECTION("aliased") {
        TestContext test("import * as Mod from 'module'");
        ImportAllStatementNode* n = ImportAllStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->moduleId == "module");
        REQUIRE(n->alias == "Mod");
    }
}

TEST_CASE("Test ImportSelectStatementNode") {
    SECTION("one import") {
        TestContext test("import { a } from 'module'");
        ImportSelectStatementNode* n = ImportSelectStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->moduleId == "module");
        REQUIRE(n->importList.size() == 1);
        REQUIRE(n->importList[0] != nullptr);
        REQUIRE(n->importList[0]->text == "a");
    }
    
    SECTION("multiple imports") {
        TestContext test("import { a, b, c } from 'module'");
        ImportSelectStatementNode* n = ImportSelectStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->moduleId == "module");
        REQUIRE(n->importList.size() == 3);
        REQUIRE(n->importList[0] != nullptr);
        REQUIRE(n->importList[0]->text == "a");
        REQUIRE(n->importList[1] != nullptr);
        REQUIRE(n->importList[1]->text == "b");
        REQUIRE(n->importList[2] != nullptr);
        REQUIRE(n->importList[2]->text == "c");
    }
}

TEST_CASE("Test ReturnStatementNode") {
    SECTION("no return value") {
        TestContext test("return");
        ReturnStatementNode* n = ReturnStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->returnValue == nullptr);
    }

    SECTION("with return value") {
        TestContext test("return var");
        ReturnStatementNode* n = ReturnStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->returnValue != nullptr);
        REQUIRE(n->returnValue->getType() == NodeType::ExpressionNode);
        REQUIRE(((ExpressionNode*)n->returnValue)->expr != nullptr);
        REQUIRE(((ExpressionNode*)n->returnValue)->expr->getType() == NodeType::IdentifierNode);
        REQUIRE(((IdentifierNode*)((ExpressionNode*)n->returnValue)->expr)->text == "var");
    }
}

TEST_CASE("Test StatementBlockNode") {
    SECTION("no statements") {
        TestContext test("{}");
        StatementBlockNode* n = StatementBlockNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->statements.size() == 0);
    }
    
    SECTION("one statement") {
        TestContext test("{ return 1; }");
        StatementBlockNode* n = StatementBlockNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->statements.size() == 1);
        REQUIRE(n->statements[0] != nullptr);
        REQUIRE(n->statements[0]->stmt != nullptr);
        REQUIRE(n->statements[0]->stmt->getType() == NodeType::ReturnStatementNode);
    }
    
    SECTION("multiple statements") {
        TestContext test("{ func(); a += 1; return a; }");
        StatementBlockNode* n = StatementBlockNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->statements.size() == 3);
        REQUIRE(n->statements[0] != nullptr);
        REQUIRE(n->statements[0]->stmt != nullptr);
        REQUIRE(n->statements[0]->stmt->getType() == NodeType::ExpressionStatementNode);
        REQUIRE(n->statements[1] != nullptr);
        REQUIRE(n->statements[1]->stmt != nullptr);
        REQUIRE(n->statements[1]->stmt->getType() == NodeType::ExpressionStatementNode);
        REQUIRE(n->statements[2] != nullptr);
        REQUIRE(n->statements[2]->stmt != nullptr);
        REQUIRE(n->statements[2]->stmt->getType() == NodeType::ReturnStatementNode);
    }
}

TEST_CASE("Test SwitchCaseNode") {
    SECTION("value without body") {
        TestContext test("case var:");
        SwitchCaseNode* n = SwitchCaseNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->value != nullptr);
        REQUIRE(n->value->expr != nullptr);
        REQUIRE(n->body == nullptr);
    }
    
    SECTION("value with body") {
        TestContext test("case var: break;");
        SwitchCaseNode* n = SwitchCaseNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->value != nullptr);
        REQUIRE(n->value->expr != nullptr);
        REQUIRE(n->body != nullptr);
        REQUIRE(n->body->stmt != nullptr);
    }
    
    SECTION("default without body") {
        TestContext test("default:");
        SwitchCaseNode* n = SwitchCaseNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->value == nullptr);
        REQUIRE(n->body == nullptr);
    }
    
    SECTION("default with body") {
        TestContext test("default: { break; }");
        SwitchCaseNode* n = SwitchCaseNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->value == nullptr);
        REQUIRE(n->body != nullptr);
        REQUIRE(n->body->stmt != nullptr);
    }
}

TEST_CASE("Test SwitchStatementNode") {
    SECTION("no cases") {
        TestContext test("switch (value) {}");
        SwitchStatementNode* n = SwitchStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->value != nullptr);
        REQUIRE(n->value->expr != nullptr);
        REQUIRE(n->value->expr->getType() == NodeType::IdentifierNode);

        REQUIRE(n->cases.size() == 0);
    }
    
    SECTION("one case") {
        TestContext test(
            "switch (value) {\n"
            "    case 1: return test;\n"
            "}"
        );
        SwitchStatementNode* n = SwitchStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->value != nullptr);
        REQUIRE(n->value->expr != nullptr);
        REQUIRE(n->value->expr->getType() == NodeType::IdentifierNode);

        REQUIRE(n->cases.size() == 1);
        REQUIRE(n->cases[0] != nullptr);
    }
    
    SECTION("multiple cases") {
        TestContext test(
            "switch (value) {\n"
            "    case 1:\n"
            "    case 2: return test + 2;\n"
            "    default: return test + 3;"
            "}"
        );
        SwitchStatementNode* n = SwitchStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->value != nullptr);
        REQUIRE(n->value->expr != nullptr);
        REQUIRE(n->value->expr->getType() == NodeType::IdentifierNode);

        REQUIRE(n->cases.size() == 3);
        REQUIRE(n->cases[0] != nullptr);
        REQUIRE(n->cases[0]->value != nullptr);
        REQUIRE(n->cases[0]->body == nullptr);
        REQUIRE(n->cases[1] != nullptr);
        REQUIRE(n->cases[1]->value != nullptr);
        REQUIRE(n->cases[1]->body != nullptr);
        REQUIRE(n->cases[2] != nullptr);
        REQUIRE(n->cases[2]->value == nullptr);
        REQUIRE(n->cases[2]->body != nullptr);
    }
}

TEST_CASE("Test ThrowStatementNode") {
    TestContext test("throw var");
    ThrowStatementNode* n = ThrowStatementNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->isError() == false);
    REQUIRE(test.ctx->getState()->messages.size() == 0);
    REQUIRE(n->value != nullptr);
    REQUIRE(n->value->expr != nullptr);
    REQUIRE(n->value->expr->getType() == NodeType::IdentifierNode);
    REQUIRE(((IdentifierNode*)n->value->expr)->text == "var");
}

TEST_CASE("Test TryCatchStatementNode") {
    SECTION("one catch block") {
        TestContext test(
            "try func();\n"
            "catch (exc: i32) {}"
        );
        TryCatchStatementNode* n = TryCatchStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->tryBody != nullptr);
        REQUIRE(n->tryBody->stmt != nullptr);
        REQUIRE(n->tryBody->stmt->getType() == NodeType::ExpressionStatementNode);

        REQUIRE(n->catchBlocks.size() == 1);
        REQUIRE(n->catchBlocks[0] != nullptr);
    }
    
    SECTION("multiple catch blocks") {
        TestContext test(
            "try func();\n"
            "catch (exc: i16) {}\n"
            "catch (exc: i64) {}\n"
            "catch (exc: i64) {}\n"
        );
        TryCatchStatementNode* n = TryCatchStatementNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->tryBody != nullptr);
        REQUIRE(n->tryBody->stmt != nullptr);
        REQUIRE(n->tryBody->stmt->getType() == NodeType::ExpressionStatementNode);

        REQUIRE(n->catchBlocks.size() == 3);
        REQUIRE(n->catchBlocks[0] != nullptr);
        REQUIRE(n->catchBlocks[0]->exception != nullptr);
        REQUIRE(n->catchBlocks[0]->exception->name == "exc");
        REQUIRE(n->catchBlocks[0]->exception->type != nullptr);
        REQUIRE(n->catchBlocks[0]->exception->type->type != nullptr);
        REQUIRE(n->catchBlocks[0]->exception->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);

        REQUIRE(n->catchBlocks[1] != nullptr);
        REQUIRE(n->catchBlocks[1]->exception != nullptr);
        REQUIRE(n->catchBlocks[1]->exception->name == "exc");
        REQUIRE(n->catchBlocks[1]->exception->type != nullptr);
        REQUIRE(n->catchBlocks[1]->exception->type->type != nullptr);
        REQUIRE(n->catchBlocks[1]->exception->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);

        REQUIRE(n->catchBlocks[2] != nullptr);
        REQUIRE(n->catchBlocks[2]->exception != nullptr);
        REQUIRE(n->catchBlocks[2]->exception->name == "exc");
        REQUIRE(n->catchBlocks[2]->exception->type != nullptr);
        REQUIRE(n->catchBlocks[2]->exception->type->type != nullptr);
        REQUIRE(n->catchBlocks[2]->exception->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
    }
}

TEST_CASE("Test WhileLoopNode") {
    TestContext test("while (cond) { }");
    WhileLoopNode* n = WhileLoopNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->isError() == false);
    REQUIRE(test.ctx->getState()->messages.size() == 0);
    REQUIRE(n->condition != nullptr);
    REQUIRE(n->condition->getType() == NodeType::ExpressionNode);
    REQUIRE(((ExpressionNode*)n->condition)->expr != nullptr);
    REQUIRE(((ExpressionNode*)n->condition)->expr->getType() == NodeType::IdentifierNode);
    REQUIRE(((IdentifierNode*)((ExpressionNode*)n->condition)->expr)->text == "cond");
    REQUIRE(n->body != nullptr);
    REQUIRE(n->body->stmt != nullptr);
    REQUIRE(n->body->stmt->getType() == NodeType::StatementBlockNode);
}

#include "Common.h"
#include <parse/declarations/ClassNode.h>
#include <parse/classes/ClassConstructorNode.h>
#include <parse/classes/ClassDestructorNode.h>
#include <parse/classes/ClassMethodNode.h>
#include <parse/classes/ClassOperatorMethodNode.h>
#include <parse/classes/ClassPropertyGetterNode.h>
#include <parse/classes/ClassPropertyNode.h>
#include <parse/classes/ClassPropertySetterNode.h>
#include <parse/misc/ParameterListNode.h>
#include <parse/misc/TypeParameterListNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/types/IdentifierTypeSpecifierNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <utils/String.h>
#include <utils/Array.hpp>

using namespace tokenize;
using namespace parse;
using namespace utils;

TEST_CASE("Test ClassConstructorNode", "[parse]") {
    SECTION("implicit public access") {
        TestContext test("constructor();");
        ClassConstructorNode* n = ClassConstructorNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == true);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("public access") {
        TestContext test("public constructor();");
        ClassConstructorNode* n = ClassConstructorNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == true);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("private access") {
        TestContext test("private constructor();");
        ClassConstructorNode* n = ClassConstructorNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("parameters") {
        ClassConstructorNode* n = nullptr;
        
        TestContext test0("constructor();");
        n = ClassConstructorNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 0);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("constructor(p1: i32);");
        n = ClassConstructorNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 1);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }

    SECTION("body") {
        ClassConstructorNode* n = nullptr;
        
        TestContext test0("constructor();");
        n = ClassConstructorNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->body == nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("constructor() {}");
        n = ClassConstructorNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->body != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
}

TEST_CASE("Test ClassDestructorNode", "[parse]") {
    SECTION("implicit public access") {
        TestContext test("destructor();");
        ClassDestructorNode* n = ClassDestructorNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == true);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("public access") {
        TestContext test("public destructor();");
        ClassDestructorNode* n = ClassDestructorNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == true);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("private access") {
        TestContext test("private destructor();");
        ClassDestructorNode* n = ClassDestructorNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("parameters") {
        ClassDestructorNode* n = nullptr;
        
        TestContext test0("destructor();");
        n = ClassDestructorNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 0);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("destructor(p1: i32);");
        n = ClassDestructorNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 1);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }

    SECTION("body") {
        ClassDestructorNode* n = nullptr;
        
        TestContext test0("destructor();");
        n = ClassDestructorNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->body == nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("destructor() {}");
        n = ClassDestructorNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->body != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
}

TEST_CASE("Test ClassMethodNode", "[parse]") {
    SECTION("implicit public access") {
        TestContext test("method() : void;");
        ClassMethodNode* n = ClassMethodNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == true);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("public access") {
        TestContext test("public method() : void;");
        ClassMethodNode* n = ClassMethodNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == true);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("private access") {
        TestContext test("private method() : void;");
        ClassMethodNode* n = ClassMethodNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == false);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("name") {
        TestContext test("private method() : void;");
        ClassMethodNode* n = ClassMethodNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE((n->name == "method"));
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("async") {
        TestContext test("async method() : void;");
        ClassMethodNode* n = ClassMethodNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isAsync == true);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("static") {
        TestContext test("static method() : void;");
        ClassMethodNode* n = ClassMethodNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isStatic == true);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("parameters") {
        ClassMethodNode* n = nullptr;
        
        TestContext test0("method() : void;");
        n = ClassMethodNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 0);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("method(p1: i32) : void;");
        n = ClassMethodNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 1);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("type parameters") {
        ClassMethodNode* n = nullptr;
        
        TestContext test0("method() : void;");
        n = ClassMethodNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->typeParameters == nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("method<P1>() : void;");
        n = ClassMethodNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->typeParameters != nullptr);
        REQUIRE(n->typeParameters->parameters.size() == 1);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("body") {
        ClassMethodNode* n = nullptr;
        
        TestContext test0("method() : void;");
        n = ClassMethodNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->body == nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("method() : void {}");
        n = ClassMethodNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->body != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }

    SECTION("return type") {
        ClassMethodNode* n = nullptr;
        
        TestContext test0("method() : void;");
        n = ClassMethodNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->returnType != nullptr);
        REQUIRE(n->returnType->type != nullptr);
        REQUIRE(n->returnType->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE((((IdentifierTypeSpecifierNode*)n->returnType->type)->name == "void"));
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("method() : i32;");
        n = ClassMethodNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->returnType != nullptr);
        REQUIRE(n->returnType->type != nullptr);
        REQUIRE(n->returnType->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE((((IdentifierTypeSpecifierNode*)n->returnType->type)->name == "i32"));
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
}

TEST_CASE("Test ClassOperatorMethodNode", "[parse]") {
    SECTION("implicit public access") {
        TestContext test("operator +() : void;");
        ClassOperatorMethodNode* n = ClassOperatorMethodNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == true);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("public access") {
        TestContext test("public operator +() : void;");
        ClassOperatorMethodNode* n = ClassOperatorMethodNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == true);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("private access") {
        TestContext test("private operator +() : void;");
        ClassOperatorMethodNode* n = ClassOperatorMethodNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == false);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("async") {
        TestContext test("async operator +() : void;");
        ClassOperatorMethodNode* n = ClassOperatorMethodNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isAsync == true);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("static") {
        TestContext test("static operator +() : void;");
        ClassOperatorMethodNode* n = ClassOperatorMethodNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isStatic == true);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("operation") {
        struct TestCase {
            const char* opStr;
            OperatorType op;
        };
        TestCase tests[] = {
            { "+(rhs: i32)", OperatorType::Add },
            { "-(rhs: i32)", OperatorType::Sub },
            { "*(rhs: i32)", OperatorType::Mul },
            { "/(rhs: i32)", OperatorType::Div },
            { "%(rhs: i32)", OperatorType::Mod },
            { "**(rhs: i32)", OperatorType::Exp },
            { "+=(rhs: i32)", OperatorType::AddEq },
            { "-=(rhs: i32)", OperatorType::SubEq },
            { "*=(rhs: i32)", OperatorType::MulEq },
            { "/=(rhs: i32)", OperatorType::DivEq },
            { "%=(rhs: i32)", OperatorType::ModEq },
            { "**=(rhs: i32)", OperatorType::ExpEq },
            { "&(rhs: i32)", OperatorType::BitwiseAnd },
            { "|(rhs: i32)", OperatorType::BitwiseOr },
            { "^(rhs: i32)", OperatorType::BitwiseXOr },
            { "~()", OperatorType::BitwiseInvert },
            { "<<(rhs: i32)", OperatorType::ShiftLeft },
            { ">>(rhs: i32)", OperatorType::ShiftRight },
            { "&&(rhs: i32)", OperatorType::LogicalAnd },
            { "||(rhs: i32)", OperatorType::LogicalOr },
            { "&=(rhs: i32)", OperatorType::BitwiseAndEq },
            { "|=(rhs: i32)", OperatorType::BitwiseOrEq },
            { "^=(rhs: i32)", OperatorType::BitwiseXOrEq },
            { "<<=(rhs: i32)", OperatorType::ShiftLeftEq },
            { ">>=(rhs: i32)", OperatorType::ShiftRightEq },
            { "&&=(rhs: i32)", OperatorType::LogicalAndEq },
            { "||=(rhs: i32)", OperatorType::LogicalOrEq },
            { "!()", OperatorType::Not },
            { "=(rhs: i32)", OperatorType::Assign },
            { "==(rhs: i32)", OperatorType::Equality },
            { "!=(rhs: i32)", OperatorType::Inequality },
            { "<(rhs: i32)", OperatorType::LessThan },
            { "<=(rhs: i32)", OperatorType::LessThanEq },
            { ">(rhs: i32)", OperatorType::GreaterThan },
            { ">=(rhs: i32)", OperatorType::GreaterThanEq },
            { "*()", OperatorType::Dereference },
            { "()()", OperatorType::Call },
            { "[](idx: i32)", OperatorType::Index },
            { "as()", OperatorType::Cast },
            { "-()", OperatorType::Negate },
            { "++()", OperatorType::PreIncrement },
            { "++(postfix)", OperatorType::PostIncrement },
            { "--()", OperatorType::PreDecrement },
            { "--(postfix)", OperatorType::PostDecrement }
        };

        u32 count = sizeof(tests) / sizeof(TestCase);
        for (u32 i = 0;i < count;i++) {
            String src = String::Format("operator %s : void;", tests[i].opStr);
            CAPTURE(tests[i].opStr);
            CAPTURE(src.c_str());

            TestContext test(src.c_str());
            ClassOperatorMethodNode* n = ClassOperatorMethodNode::TryParse(test.ctx);
            REQUIRE(n != nullptr);
            REQUIRE(n->operation == tests[i].op);
            REQUIRE(n->isError() == false);
            REQUIRE(test.ctx->getState()->messages.size() == 0);
        }
    }

    SECTION("parameters") {
        ClassOperatorMethodNode* n = nullptr;
        
        TestContext test0("operator +() : void;");
        n = ClassOperatorMethodNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters.size() == 0);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("operator +(p1: i32) : void;");
        n = ClassOperatorMethodNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters.size() == 1);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);

        TestContext test2("operator +(p1: i8, p2: i16, p3: i32) : void;");
        n = ClassOperatorMethodNode::TryParse(test2.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters.size() == 3);
        REQUIRE(n->isError() == false);
        REQUIRE(test2.ctx->getState()->messages.size() == 0);

        REQUIRE((n->parameters[0]->name == "p1"));
        REQUIRE(n->parameters[0]->type != nullptr);
        REQUIRE(n->parameters[0]->type->type != nullptr);
        REQUIRE(n->parameters[0]->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE((((IdentifierTypeSpecifierNode*)n->parameters[0]->type->type)->name) == "i8");

        REQUIRE((n->parameters[1]->name == "p2"));
        REQUIRE(n->parameters[1]->type != nullptr);
        REQUIRE(n->parameters[1]->type->type != nullptr);
        REQUIRE(n->parameters[1]->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE((((IdentifierTypeSpecifierNode*)n->parameters[1]->type->type)->name) == "i16");

        REQUIRE((n->parameters[2]->name == "p3"));
        REQUIRE(n->parameters[2]->type != nullptr);
        REQUIRE(n->parameters[2]->type->type != nullptr);
        REQUIRE(n->parameters[2]->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE((((IdentifierTypeSpecifierNode*)n->parameters[2]->type->type)->name) == "i32");
    }
    
    SECTION("type parameters") {
        ClassOperatorMethodNode* n = nullptr;
        
        TestContext test0("operator +() : void;");
        n = ClassOperatorMethodNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->typeParameters == nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("operator +<P1>() : void;");
        n = ClassOperatorMethodNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->typeParameters != nullptr);
        REQUIRE(n->typeParameters->parameters.size() == 1);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }

    SECTION("body") {
        ClassOperatorMethodNode* n = nullptr;
        
        TestContext test0("operator +() : void;");
        n = ClassOperatorMethodNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->body == nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("operator +() : void {}");
        n = ClassOperatorMethodNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->body != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }

    SECTION("return type") {
        ClassOperatorMethodNode* n = nullptr;
        
        TestContext test0("operator +() : void;");
        n = ClassOperatorMethodNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->returnType != nullptr);
        REQUIRE(n->returnType->type != nullptr);
        REQUIRE(n->returnType->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE((((IdentifierTypeSpecifierNode*)n->returnType->type)->name == "void"));
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("operator +() : i32;");
        n = ClassOperatorMethodNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->returnType != nullptr);
        REQUIRE(n->returnType->type != nullptr);
        REQUIRE(n->returnType->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE((((IdentifierTypeSpecifierNode*)n->returnType->type)->name == "i32"));
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
}

TEST_CASE("Test ClassPropertyGetterNode", "[parse]") {
    SECTION("implicit public access") {
        TestContext test("get prop() : void;");
        ClassPropertyGetterNode* n = ClassPropertyGetterNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == true);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("public access") {
        TestContext test("public get prop() : void;");
        ClassPropertyGetterNode* n = ClassPropertyGetterNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == true);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("private access") {
        TestContext test("private get prop() : void;");
        ClassPropertyGetterNode* n = ClassPropertyGetterNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == false);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("property name") {
        TestContext test("private get prop() : void;");
        ClassPropertyGetterNode* n = ClassPropertyGetterNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE((n->propertyName == "prop"));
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("async") {
        TestContext test("async get prop() : void;");
        ClassPropertyGetterNode* n = ClassPropertyGetterNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isAsync == true);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("static") {
        TestContext test("static get prop() : void;");
        ClassPropertyGetterNode* n = ClassPropertyGetterNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isStatic == true);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("parameters") {
        ClassPropertyGetterNode* n = nullptr;
        
        TestContext test0("get prop() : void;");
        n = ClassPropertyGetterNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 0);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("get prop(p1: i32) : void;");
        n = ClassPropertyGetterNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 1);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("body") {
        ClassPropertyGetterNode* n = nullptr;
        
        TestContext test0("get prop() : void;");
        n = ClassPropertyGetterNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->body == nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("get prop() : void {}");
        n = ClassPropertyGetterNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->body != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }

    SECTION("return type") {
        ClassPropertyGetterNode* n = nullptr;
        
        TestContext test0("get prop() : void;");
        n = ClassPropertyGetterNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->returnType != nullptr);
        REQUIRE(n->returnType->type != nullptr);
        REQUIRE(n->returnType->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE((((IdentifierTypeSpecifierNode*)n->returnType->type)->name == "void"));
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("get prop() : i32;");
        n = ClassPropertyGetterNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->returnType != nullptr);
        REQUIRE(n->returnType->type != nullptr);
        REQUIRE(n->returnType->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE((((IdentifierTypeSpecifierNode*)n->returnType->type)->name == "i32"));
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
}

TEST_CASE("Test ClassPropertyNode", "[parse]") {
    SECTION("implicit public access") {
        TestContext test("prop : void;");
        ClassPropertyNode* n = ClassPropertyNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == true);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("public access") {
        TestContext test("public prop : void;");
        ClassPropertyNode* n = ClassPropertyNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == true);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("private access") {
        TestContext test("private prop : void;");
        ClassPropertyNode* n = ClassPropertyNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == false);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("property name") {
        TestContext test("private prop : void;");
        ClassPropertyNode* n = ClassPropertyNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE((n->name == "prop"));
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("property type") {
        TestContext test("private prop : void;");
        ClassPropertyNode* n = ClassPropertyNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->type != nullptr);
        REQUIRE(n->type->type != nullptr);
        REQUIRE(n->type->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE((((IdentifierTypeSpecifierNode*)n->type->type)->name == "void"));
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("static") {
        TestContext test("static prop : void;");
        ClassPropertyNode* n = ClassPropertyNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isStatic == true);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
}

TEST_CASE("Test ClassPropertySetterNode", "[parse]") {
    SECTION("implicit public access") {
        TestContext test("set prop() : void;");
        ClassPropertySetterNode* n = ClassPropertySetterNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == true);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("public access") {
        TestContext test("public set prop() : void;");
        ClassPropertySetterNode* n = ClassPropertySetterNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == true);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("private access") {
        TestContext test("private set prop() : void;");
        ClassPropertySetterNode* n = ClassPropertySetterNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isPublic == false);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("property name") {
        TestContext test("private set prop() : void;");
        ClassPropertySetterNode* n = ClassPropertySetterNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE((n->propertyName == "prop"));
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("async") {
        TestContext test("async set prop() : void;");
        ClassPropertySetterNode* n = ClassPropertySetterNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isAsync == true);
        REQUIRE(n->isStatic == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("static") {
        TestContext test("static set prop() : void;");
        ClassPropertySetterNode* n = ClassPropertySetterNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isStatic == true);
        REQUIRE(n->isAsync == false);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("parameters") {
        ClassPropertySetterNode* n = nullptr;
        
        TestContext test0("set prop() : void;");
        n = ClassPropertySetterNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 0);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("set prop(p1: i32) : void;");
        n = ClassPropertySetterNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 1);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
    
    SECTION("type parameters") {
        ClassPropertySetterNode* n = nullptr;
        
        TestContext test0("set prop() : void;");
        n = ClassPropertySetterNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->typeParameters == nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("set prop<P1>() : void;");
        n = ClassPropertySetterNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->typeParameters != nullptr);
        REQUIRE(n->typeParameters->parameters.size() == 1);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }

    SECTION("body") {
        ClassPropertySetterNode* n = nullptr;
        
        TestContext test0("set prop() : void;");
        n = ClassPropertySetterNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->body == nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("set prop() : void {}");
        n = ClassPropertySetterNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->body != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }

    SECTION("return type") {
        ClassPropertySetterNode* n = nullptr;
        
        TestContext test0("set prop() : void;");
        n = ClassPropertySetterNode::TryParse(test0.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->returnType != nullptr);
        REQUIRE(n->returnType->type != nullptr);
        REQUIRE(n->returnType->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE((((IdentifierTypeSpecifierNode*)n->returnType->type)->name == "void"));
        REQUIRE(n->isError() == false);
        REQUIRE(test0.ctx->getState()->messages.size() == 0);

        TestContext test1("set prop() : i32;");
        n = ClassPropertySetterNode::TryParse(test1.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->returnType != nullptr);
        REQUIRE(n->returnType->type != nullptr);
        REQUIRE(n->returnType->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE((((IdentifierTypeSpecifierNode*)n->returnType->type)->name == "i32"));
        REQUIRE(n->isError() == false);
        REQUIRE(test1.ctx->getState()->messages.size() == 0);
    }
}

TEST_CASE("Test ClassNode", "[parse]") {
    SECTION("declaration") {
        TestContext test("class Test;");
        ClassNode* n = ClassNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE((n->name == "Test"));
        REQUIRE(n->isDeclaration == true);
        REQUIRE(n->ctors.size() == 0);
        REQUIRE(n->dtor == nullptr);
        REQUIRE(n->extends.size() == 0);
        REQUIRE(n->getters.size() == 0);
        REQUIRE(n->setters.size() == 0);
        REQUIRE(n->methods.size() == 0);
        REQUIRE(n->operators.size() == 0);
        REQUIRE(n->properties.size() == 0);
        REQUIRE(n->typeParameters == nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }

    SECTION("definition") {
        TestContext test(
            "class Test<P1> extends Extends0, Extends1<P1> {\n"
            "    private prop1: i32;\n"
            "    public prop2: i32;\n"
            "\n"
            "    constructor() {}\n"
            "    constructor(p1: i32) {}\n"
            "    destructor() {}\n"
            "\n"
            "    get p1() : i32 { return this.prop1; }\n"
            "    get p2() : i32 { return this.prop2; }\n"
            "    set p1(p: i32) : i32 { return this.prop1 = p; }\n"
            "    set p2(p: i32) : i32 { return this.prop2 = p; }\n"
            "\n"
            "    method0() : void {}\n"
            "    method1() : void {}\n"
            "\n"
            "    operator+(rhs: i32) : i32 { return this.prop1 + rhs; }\n"
            "    operator-(rhs: i32) : i32 { return this.prop1 - rhs; }\n"
            "}"
        );

        ClassNode* n = ClassNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE((n->name == "Test"));
        REQUIRE(n->isDeclaration == false);
        REQUIRE(n->ctors.size() == 2);
        REQUIRE(n->dtor != nullptr);
        REQUIRE(n->extends.size() == 2);
        REQUIRE(n->getters.size() == 2);
        REQUIRE(n->setters.size() == 2);
        REQUIRE(n->methods.size() == 2);
        REQUIRE(n->operators.size() == 2);
        REQUIRE(n->properties.size() == 2);
        REQUIRE(n->typeParameters != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
    }
}
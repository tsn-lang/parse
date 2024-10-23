#include "Common.h"
#include <parse/proxies/ExpressionNode.h>
#include <parse/proxies/StatementNode.h>
#include <parse/proxies/TopLevelNode.h>
#include <utils/Array.hpp>

using namespace tokenize;
using namespace parse; 

TEST_CASE("Test ExpressionNode", "[parse]") {
    struct TestCase {
        Array<const char*> codeStrs;
        NodeType expectedType;
    };

    TestCase tests[] = {
        // Primary expressions
        { { "typeinfo<Test>" }, NodeType::TypeInfoExpressionNode },
        { { "sizeof<Test>" }, NodeType::SizeOfExpressionNode },
        { { "(1 + 1)" }, NodeType::ExpressionNode },
        { { "(1, 2, 3)" }, NodeType::ExpressionSequenceNode },
        { { "this" }, NodeType::ThisExpressionNode },
        { { "'str'" }, NodeType::StringLiteralNode },
        { { "`tstr`" }, NodeType::TemplateStringLiteralNode },
        { { "1ub", "1us", "1ul", "1ull", "1b", "1s", "1l", "1ll", "0xDEADBEEF", "0b10101010" }, NodeType::NumberLiteralNode },
        { { "true", "false" }, NodeType::BooleanLiteralNode },
        { { "null" }, NodeType::NullLiteralNode },
        { { "{ a: 1, b: 2, c: 3 }", "{}" }, NodeType::ObjectLiteralNode },
        { { "[1, 2, 3]", "[]" }, NodeType::ArrayLiteralNode },
        { { "test" }, NodeType::IdentifierNode },
        { { "Test<i32>" }, NodeType::IdentifierTypeSpecifierNode },

        // The rest
        { { "function(){}" }, NodeType::FunctionExpressionNode },
        { { "() => {}", "() => 1" }, NodeType::ArrowFunctionExpressionNode },
        { { "a.b", "Test<i32>.b" }, NodeType::MemberExpressionNode },
        { { "a[b]", "Test<i32>[b]" }, NodeType::IndexExpressionNode },
        { { "new Test()", "new Test() => dest" }, NodeType::NewExpressionNode },
        { { "test()", "test.method()", "Test<i32>.staticMethod()" }, NodeType::CallExpressionNode },
        { { "i++", "i--", "++i", "--i", "~i", "!i", "-i", "*i" }, NodeType::UnaryExpressionNode },
        { { "test as Test" }, NodeType::CastExpressionNode },
        {
            {
                "a ** b", "a * b", "a / b", "a % b", "a + b", "a - b",
                "a << b", "a >> b", "a < b", "a > b", "a <= b", "a >= b",
                "a == b", "a != b", "a & b", "a ^ b", "a | b", "a && b",
                "a || b", "a = b", "a += b", "a -= b", "a *= b", "a /= b",
                "a %= b", "a **= b", "a &= b", "a |= b", "a ^= b", "a <<= b",
                "a >>= b", "a &&= b", "a ||= b"
            },
            NodeType::BinaryExpressionNode
        },
        { { "a ? b : c" }, NodeType::ConditionalExpressionNode },
        { { "await test" }, NodeType::AwaitExpressionNode }
    };

    u32 testCount = sizeof(tests) / sizeof(TestCase);
    for (u32 i = 0;i < testCount;i++) {
        CAPTURE(i);
        for (u32 t = 0;t < tests[i].codeStrs.size();t++) {
            CAPTURE(tests[i].codeStrs[t]);

            TestContext test(tests[i].codeStrs[t]);
            ExpressionNode* n = ExpressionNode::TryParse(test.ctx);
            REQUIRE(n != nullptr);
            REQUIRE(n->expr != nullptr);
            REQUIRE(n->expr->getType() == tests[i].expectedType);
            REQUIRE(n->isError() == false);
            REQUIRE(test.ctx->getState()->messages.size() == 0);
        }
    }
}

TEST_CASE("Test StatementNode", "[parse]") {
    struct TestCase {
        Array<const char*> codeStrs;
        NodeType expectedType;
    };

    TestCase tests[] = {
        { { ";" }, NodeType::EmptyStatementNode },
        { { "break;" }, NodeType::BreakStatementNode },
        { { "continue;" }, NodeType::ContinueStatementNode },
        { { "return;", "return value;" }, NodeType::ReturnStatementNode },
        { { "throw exc;" }, NodeType::ThrowStatementNode },
        { { "delete val;" }, NodeType::DeleteStatementNode },
        {
            {
                "const value;",
                "const value : i32;",
                "const { a, b, c };",
                "const [ a, b, c ];",
                "const value = val;",
                "const value : i32 = val;",
                "const { a, b, c } = val;",
                "const [ a, b, c ] = val;",
                "let value;",
                "let value : i32;",
                "let { a, b, c };",
                "let [ a, b, c ];",
                "let value = val;",
                "let value : i32 = val;",
                "let { a, b, c } = val;",
                "let [ a, b, c ] = val;"
            },
            NodeType::DeclarationStatementNode
        },
        { { "test();" }, NodeType::ExpressionStatementNode },
        {
            {
                "if (cond) test();",
                "if (cond) { test(); }",
                "if (cond) {}"
            },
            NodeType::IfStatementNode
        },
        {
            {
                "for (let i = 0; i < 10;i++) test();",
                "for (let i = 0; i < 10;i++) { test(); }",
                "for (let i = 0; i < 10;i++) { }",
            },
            NodeType::ForLoopNode
        },
        {
            {
                "switch (value) {}",
                "switch (value) { case 0: break; }",
                "switch (value) { case 0: test(); default: break; }",
            },
            NodeType::SwitchStatementNode
        },
        {
            {
                "while (cond) test();",
                "while (cond) { test(); }",
                "while (cond) { }",
            },
            NodeType::WhileLoopNode
        },
        {
            {
                "do test(); while (cond)",
                "do { test(); } while (cond)",
                "do { } while (cond)",
            },
            NodeType::DoWhileLoopNode
        },
        {
            {
                "try test(); catch (exc: i32) test();",
                "try test(); catch (exc: i32) { test(); }",
                "try test(); catch (exc: i32) { }",
                "try { test(); } catch (exc: i32) test();",
                "try { test(); } catch (exc: i32) { test(); }",
                "try { test(); } catch (exc: i32) { }",
                "try { } catch (exc: i32) test();",
                "try { } catch (exc: i32) { test(); }",
                "try { } catch (exc: i32) { }",
            },
            NodeType::TryCatchStatementNode
        },
        { { "{}" }, NodeType::StatementBlockNode }
    };

    u32 testCount = sizeof(tests) / sizeof(TestCase);
    for (u32 i = 0;i < testCount;i++) {
        CAPTURE(i);
        for (u32 t = 0;t < tests[i].codeStrs.size();t++) {
            CAPTURE(tests[i].codeStrs[t]);

            TestContext test(tests[i].codeStrs[t]);
            StatementNode* n = StatementNode::TryParse(test.ctx);
            REQUIRE(n != nullptr);
            REQUIRE(n->stmt != nullptr);
            REQUIRE(n->stmt->getType() == tests[i].expectedType);
            REQUIRE(n->isError() == false);
            REQUIRE(test.ctx->getState()->messages.size() == 0);
        }
    }
}

TEST_CASE("Test TopLevelNode", "[parse]") {
    struct TestCase {
        Array<const char*> codeStrs;
        NodeType expectedType;
    };

    TestCase tests[] = {
        { { "function func() : i32;" }, NodeType::FunctionNode },
        { { "class Cls;" }, NodeType::ClassNode },
        { { "type Type;" }, NodeType::TypeNode },
        { { "enum Enum;" }, NodeType::EnumNode },
        { { "export var;" }, NodeType::ExportStatementNode },
        { { "import * from 'module';" }, NodeType::ImportAllStatementNode },
        { { "import { a, b, c } from 'module';" }, NodeType::ImportSelectStatementNode },
        { { "func();" }, NodeType::StatementNode }
    };

    u32 testCount = sizeof(tests) / sizeof(TestCase);
    for (u32 i = 0;i < testCount;i++) {
        CAPTURE(i);
        for (u32 t = 0;t < tests[i].codeStrs.size();t++) {
            CAPTURE(tests[i].codeStrs[t]);

            TestContext test(tests[i].codeStrs[t]);
            TopLevelNode* n = TopLevelNode::TryParse(test.ctx);
            REQUIRE(n != nullptr);
            REQUIRE(n->statement != nullptr);
            REQUIRE(n->statement->getType() == tests[i].expectedType);
            REQUIRE(n->isError() == false);
            REQUIRE(test.ctx->getState()->messages.size() == 0);
        }
    }
}

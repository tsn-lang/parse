#include "Common.h"
#include <parse/types/ArrayTypeNode.h>
#include <parse/types/FunctionTypeNode.h>
#include <parse/types/IdentifierTypeSpecifierNode.h>
#include <parse/types/ObjectTypeNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/misc/ParameterListNode.h>
#include <utils/Array.hpp>

using namespace tokenize;
using namespace parse; 

TEST_CASE("Test ArrayTypeNode", "[parse]") {
    SECTION("no elements") {
        TestContext test("[]");
        ArrayTypeNode* n = ArrayTypeNode::TryParse(test.ctx);
        REQUIRE(n == nullptr);
    }
    
    SECTION("one element") {
        TestContext test("[a: i32]");
        ArrayTypeNode* n = ArrayTypeNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->elements.size() == 1);
        REQUIRE(n->elements[0] != nullptr);
        REQUIRE(n->elements[0]->name == "a");
        REQUIRE(n->elements[0]->type != nullptr);
    }
    
    SECTION("multiple elements") {
        TestContext test("[a: i32, b: i32, c: i64]");
        ArrayTypeNode* n = ArrayTypeNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->elements.size() == 3);
        REQUIRE(n->elements[0] != nullptr);
        REQUIRE(n->elements[0]->name == "a");
        REQUIRE(n->elements[0]->type != nullptr);
        REQUIRE(n->elements[1] != nullptr);
        REQUIRE(n->elements[1]->name == "b");
        REQUIRE(n->elements[1]->type != nullptr);
        REQUIRE(n->elements[2] != nullptr);
        REQUIRE(n->elements[2]->name == "c");
        REQUIRE(n->elements[2]->type != nullptr);
    }
}

TEST_CASE("Test FunctionTypeNode", "[parse]") {
    SECTION("no parameters") {
        TestContext test("() => void");
        FunctionTypeNode* n = FunctionTypeNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->returnType != nullptr);
    }
    
    SECTION("with parameters") {
        TestContext test("(a: i32, b: i32) => i32");
        FunctionTypeNode* n = FunctionTypeNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->parameters != nullptr);
        REQUIRE(n->parameters->parameters.size() == 2);
        REQUIRE(n->parameters->parameters[0] != nullptr);
        REQUIRE(n->parameters->parameters[1] != nullptr);
        REQUIRE(n->returnType != nullptr);
    }
}

TEST_CASE("Test IdentifierTypeSpecifierNode", "[parse]") {
    SECTION("just identifier") {
        TestContext test("Test");
        IdentifierTypeSpecifierNode* n = IdentifierTypeSpecifierNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->name == "Test");
        REQUIRE(n->parameters.size() == 0);
    }
    
    SECTION("one parameter") {
        TestContext test("Test<P1>");
        IdentifierTypeSpecifierNode* n = IdentifierTypeSpecifierNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->name == "Test");
        REQUIRE(n->parameters.size() == 1);
        REQUIRE(n->parameters[0] != nullptr);
        REQUIRE(n->parameters[0]->type != nullptr);
        REQUIRE(n->parameters[0]->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->parameters[0]->type)->name == "P1");
    }
    
    SECTION("multiple parameters") {
        TestContext test("Test<P1, P2, P3>");
        IdentifierTypeSpecifierNode* n = IdentifierTypeSpecifierNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);
        REQUIRE(n->name == "Test");
        REQUIRE(n->parameters.size() == 3);
        REQUIRE(n->parameters[0] != nullptr);
        REQUIRE(n->parameters[0]->type != nullptr);
        REQUIRE(n->parameters[0]->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->parameters[0]->type)->name == "P1");
        REQUIRE(n->parameters[1] != nullptr);
        REQUIRE(n->parameters[1]->type != nullptr);
        REQUIRE(n->parameters[1]->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->parameters[1]->type)->name == "P2");
        REQUIRE(n->parameters[2] != nullptr);
        REQUIRE(n->parameters[2]->type != nullptr);
        REQUIRE(n->parameters[2]->type->getType() == NodeType::IdentifierTypeSpecifierNode);
        REQUIRE(((IdentifierTypeSpecifierNode*)n->parameters[2]->type)->name == "P3");
    }
}

TEST_CASE("Test ObjectTypeNode", "[parse]") {
    SECTION("no properties") {
        TestContext test("{}");
        ObjectTypeNode* n = ObjectTypeNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->properties.size() == 0);
    }
    
    SECTION("one property") {
        TestContext test("{ a: i32; }");
        ObjectTypeNode* n = ObjectTypeNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->properties.size() == 1);
        REQUIRE(n->properties[0] != nullptr);
        REQUIRE(n->properties[0]->name == "a");
        REQUIRE(n->properties[0]->type != nullptr);
    }
    
    SECTION("multiple properties") {
        TestContext test("{ a: i32; b: i32; c: i64; }");
        ObjectTypeNode* n = ObjectTypeNode::TryParse(test.ctx);
        REQUIRE(n != nullptr);
        REQUIRE(n->isError() == false);
        REQUIRE(test.ctx->getState()->messages.size() == 0);

        REQUIRE(n->properties.size() == 3);
        REQUIRE(n->properties[0] != nullptr);
        REQUIRE(n->properties[0]->name == "a");
        REQUIRE(n->properties[0]->type != nullptr);
        REQUIRE(n->properties[1] != nullptr);
        REQUIRE(n->properties[1]->name == "b");
        REQUIRE(n->properties[1]->type != nullptr);
        REQUIRE(n->properties[2] != nullptr);
        REQUIRE(n->properties[2]->name == "c");
        REQUIRE(n->properties[2]->type != nullptr);
    }
}

TEST_CASE("Test TypeSpecifierNode", "[parse]") {
    SECTION("proxies") {
        struct TestCase {
            Array<const char*> codeStrs;
            NodeType expectedType;
        };

        TestCase tests[] = {
            {
                {
                    "Test", "Test<P1>", "Test<P1, P2>",
                    "(Test)", "(Test<P1>)", "(Test<P1, P2>)"
                },
                NodeType::IdentifierTypeSpecifierNode
            }, {
                {
                    "{}", "{ a: i32; }", "{ a: i32; b: u32; }",
                    "({})", "({ a: i32; })", "({ a: i32; b: u32; })"
                },
                NodeType::ObjectTypeNode
            }, {
                {
                    "[a: i32]", "[a: i32, b: u32]",
                    "([a: i32])", "([a: i32, b: u32])"
                },
                NodeType::ArrayTypeNode
            }, {
                {
                    "() => void", "(a: i32) => i32",
                    "(() => void)", "((a: i32) => i32)"
                },
                NodeType::FunctionTypeNode
             }
        };

        u32 testCount = sizeof(tests) / sizeof(TestCase);
        for (u32 i = 0;i < testCount;i++) {
            CAPTURE(i);
            for (u32 t = 0;t < tests[i].codeStrs.size();t++) {
                CAPTURE(tests[i].codeStrs[t]);

                TestContext test(tests[i].codeStrs[t]);
                TypeSpecifierNode* n = TypeSpecifierNode::TryParse(test.ctx);
                REQUIRE(n != nullptr);
                REQUIRE(n->type != nullptr);
                REQUIRE(n->type->getType() == tests[i].expectedType);
                REQUIRE(n->isError() == false);
                REQUIRE(test.ctx->getState()->messages.size() == 0);
            }
        }
    }

    SECTION("modifiers") {
        struct TestCaseElem {
            NodeType type;
            TypeSpecifierNode::Modifier mod;
        };

        struct TestCase {
            const char* codeStr;
            Array<TestCaseElem> nodes;
        };

        TestCase tests[] = {
            {
                "Test",
                {
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::None },
                    { NodeType::IdentifierTypeSpecifierNode }
                }
            }, {
                "Test*",
                {
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::Pointer },
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::None },
                    { NodeType::IdentifierTypeSpecifierNode }
                }
            }, {
                "Test**",
                {
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::Pointer },
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::Pointer },
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::None },
                    { NodeType::IdentifierTypeSpecifierNode }
                }
            }, {
                "Test*[]",
                {
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::Array },
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::Pointer },
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::None },
                    { NodeType::IdentifierTypeSpecifierNode }
                }
            }, {
                "Test[]*",
                {
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::Pointer },
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::Array },
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::None },
                    { NodeType::IdentifierTypeSpecifierNode }
                }
            }, {
                "(Test)",
                {
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::None },
                    { NodeType::IdentifierTypeSpecifierNode }
                }
            }, {
                "(Test*)",
                {
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::Pointer },
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::None },
                    { NodeType::IdentifierTypeSpecifierNode }
                }
            }, {
                "(Test**)",
                {
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::Pointer },
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::Pointer },
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::None },
                    { NodeType::IdentifierTypeSpecifierNode }
                }
            }, {
                "(Test*[])",
                {
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::Array },
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::Pointer },
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::None },
                    { NodeType::IdentifierTypeSpecifierNode }
                }
            }, {
                "(Test[]*)",
                {
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::Pointer },
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::Array },
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::None },
                    { NodeType::IdentifierTypeSpecifierNode }
                }
            }, {
                "() => i32[]",
                {
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::None },
                    { NodeType::FunctionTypeNode }
                }
            }, {
                "() => i32*",
                {
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::None },
                    { NodeType::FunctionTypeNode }
                }
            }, {
                "(() => i32)[]",
                {
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::Array },
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::None },
                    { NodeType::FunctionTypeNode }
                }
            }, {
                "(() => i32)*",
                {
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::Pointer },
                    { NodeType::TypeSpecifierNode, TypeSpecifierNode::Modifier::None },
                    { NodeType::FunctionTypeNode }
                }
            }
        };

        u32 testCount = sizeof(tests) / sizeof(TestCase);
        for (u32 i = 0;i < testCount;i++) {
            CAPTURE(i);
            CAPTURE(tests[i].codeStr);
            TestContext test(tests[i].codeStr);
            Node* n = TypeSpecifierNode::TryParse(test.ctx);
            REQUIRE(test.ctx->getState()->messages.size() == 0);

            for (u32 t = 0;t < tests[i].nodes.size();t++) {
                CAPTURE(t);
                REQUIRE(n != nullptr);
                REQUIRE(n->getType() == tests[i].nodes[t].type);
                REQUIRE(n->isError() == false);
                
                switch (tests[i].nodes[t].type) {
                    case NodeType::TypeSpecifierNode: {
                        TypeSpecifierNode* tp = ((TypeSpecifierNode*)n);
                        REQUIRE(tp->mod == tests[i].nodes[t].mod);
                        n = tp->type;
                        break;
                    }
                    default: break;
                }
            }
        }
    }
}
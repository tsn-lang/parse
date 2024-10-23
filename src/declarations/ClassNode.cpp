#include <parse/declarations/ClassNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/classes/ClassPropertyNode.h>
#include <parse/classes/ClassMethodNode.h>
#include <parse/classes/ClassConstructorNode.h>
#include <parse/classes/ClassDestructorNode.h>
#include <parse/classes/ClassOperatorMethodNode.h>
#include <parse/classes/ClassPropertyGetterNode.h>
#include <parse/classes/ClassPropertySetterNode.h>
#include <parse/misc/TypeParameterListNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>
#include <utils/Array.hpp>

namespace parse {
    ClassNode::ClassNode(Context* ctx) : Node(ctx, NodeType::ClassNode), isDeclaration(false), dtor(nullptr), typeParameters(nullptr) {}
    ClassNode::~ClassNode() {}
    void ClassNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ClassNode* ClassNode::Create(Context* ctx) { return new (ctx->allocNode()) ClassNode(ctx); }

    ClassNode* ClassNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Class)) return nullptr;

        ClassNode* n = Create(ctx);
        ctx->consume(n);

        if (!ctx->match(TokenType::Identifier)) {
            ctx->logError("Expected identifier");
            n->m_isError = true;
            return n;
        }

        const Token* nameTok = ctx->get();
        n->name = nameTok->toString();
        n->extendLocation(nameTok);
        ctx->consume();

        n->typeParameters = TypeParameterListNode::TryParse(ctx);
        if (n->typeParameters && n->typeParameters->isError()) {
            n->m_isError = true;

            if (!ctx->skipToAnyMatched({
                Match(TokenType::Keyword, TokenSubType::Keyword_Extends),
                Match(TokenType::Symbol, TokenSubType::Symbol_OpenBrace),
                Match(TokenType::EndOfStatement)
            })) return n;
        }
        
        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Extends)) {
            ctx->consume(n);

            TypeSpecifierNode* ext = TypeSpecifierNode::TryParse(ctx);
            while (ext) {
                if (ext->isError()) {
                    ext->destroy();
                    ctx->skipToAnyMatched({
                        Match(TokenType::Symbol, TokenSubType::Symbol_OpenBrace),
                        Match(TokenType::EndOfStatement)
                    });
                    break;
                }

                n->extends.push(ext);
                n->extendLocation(ext);

                if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Comma)) {
                    ctx->consume(n);
                    ext = TypeSpecifierNode::TryParse(ctx);

                    if (!ext) {
                        ctx->logError("Expected type specifier after ','");
                        n->m_isError = true;
                        if (!ctx->skipToAnyMatched({
                            Match(TokenType::Symbol, TokenSubType::Symbol_OpenBrace),
                            Match(TokenType::EndOfStatement)
                        })) return n;

                        break;
                    }
                } else ext = nullptr;
            }
        }

        if (ctx->match(TokenType::EndOfStatement) || ctx->atEnd()) {
            n->isDeclaration = true;
            return n;
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenBrace)) {
            ctx->logError("Expected class body");
            n->m_isError = true;
            return n;
        }
        
        ctx->consume(n);

        bool didMatchCloseBrace = false;
        while (!ctx->atEnd()) {
            u32 foundCount = 0;
            
            ClassPropertyNode* prop = ClassPropertyNode::TryParse(ctx);
            if (prop) {
                if (prop->isError()) {
                    prop->destroy();
                    ctx->skipTo(TokenType::EndOfStatement);
                } else {
                    foundCount++;
                    n->properties.push(prop);
                    n->extendLocation(prop);
                }
            }

            ClassConstructorNode* ctor = ClassConstructorNode::TryParse(ctx);
            if (ctor) {
                if (ctor->isError()) {
                    ctor->destroy();
                    ctx->skipToAnyMatched({
                        Match(TokenType::EndOfStatement),
                        Match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)
                    });
                } else {
                    foundCount++;
                    n->ctors.push(ctor);
                    n->extendLocation(ctor);
                }
            }

            ClassDestructorNode* dtor = ClassDestructorNode::TryParse(ctx);
            if (dtor) {
                if (dtor->isError()) {
                    dtor->destroy();
                    ctx->skipToAnyMatched({
                        Match(TokenType::EndOfStatement),
                        Match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)
                    });
                } else {
                    foundCount++;
                    if (n->dtor) {
                        ctx->logError("Destructor for class '%s' already exists", n->name.c_str());
                        n->m_isError = true;
                    }

                    n->dtor = dtor;
                    n->extendLocation(dtor);
                }
            }

            ClassMethodNode* meth = ClassMethodNode::TryParse(ctx);
            if (meth) {
                if (meth->isError()) {
                    meth->destroy();
                    ctx->skipToAnyMatched({
                        Match(TokenType::EndOfStatement),
                        Match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)
                    });
                } else {
                    foundCount++;
                    n->methods.push(meth);
                    n->extendLocation(meth);
                }
            }

            ClassPropertyGetterNode* getter = ClassPropertyGetterNode::TryParse(ctx);
            if (getter) {
                if (getter->isError()) {
                    getter->destroy();
                    ctx->skipToAnyMatched({
                        Match(TokenType::EndOfStatement),
                        Match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)
                    });
                } else {
                    foundCount++;
                    n->getters.push(getter);
                    n->extendLocation(getter);
                }
            }

            ClassPropertySetterNode* setter = ClassPropertySetterNode::TryParse(ctx);
            if (setter) {
                if (setter->isError()) {
                    setter->destroy();
                    ctx->skipToAnyMatched({
                        Match(TokenType::EndOfStatement),
                        Match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)
                    });
                } else {
                    foundCount++;
                    n->setters.push(setter);
                    n->extendLocation(setter);
                }
            }

            ClassOperatorMethodNode* op = ClassOperatorMethodNode::TryParse(ctx);
            if (op) {
                if (op->isError()) {
                    op->destroy();
                    ctx->skipToAnyMatched({
                        Match(TokenType::EndOfStatement),
                        Match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)
                    });
                } else {
                    foundCount++;
                    n->operators.push(op);
                    n->extendLocation(op);
                }
            }

            if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)) {
                ctx->consume(n);
                didMatchCloseBrace = true;
                break;
            }
            
            if (foundCount == 0 && !ctx->atEnd()) {
                ctx->logError("Unexpected token '%s'", ctx->get()->toString().c_str());
                ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseBrace);
            }
        }
        
        if (!didMatchCloseBrace) {
            ctx->logError("Expected '}'");
            n->m_isError = true;
        }

        return n;
    }
};
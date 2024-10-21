#include <parse/classes/ClassMethodNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/misc/TypeParameterListNode.h>
#include <parse/misc/ParameterListNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    ClassMethodNode::ClassMethodNode(Context* ctx)
        : Node(ctx, NodeType::ClassMethodNode), isPublic(true), isStatic(false), isAsync(false), returnType(nullptr),
        parameters(nullptr), body(nullptr), typeParameters(nullptr)
    {}
    ClassMethodNode::~ClassMethodNode() {}
    void ClassMethodNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ClassMethodNode* ClassMethodNode::Create(Context* ctx) { return new (ctx->allocNode()) ClassMethodNode(ctx); }

    ClassMethodNode* ClassMethodNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        ctx->begin();

        ClassMethodNode* n = Create(ctx);

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Private)) {
            ctx->consume(n);

            n->isPublic = false;
        } else if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Public)) {
            ctx->consume(n);

            n->isPublic = true;
        }

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Static)) {
            ctx->consume(n);

            n->isStatic = true;
        }

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Async)) {
            ctx->consume(n);

            n->isAsync = true;
        }
        
        if (!ctx->match(TokenType::Identifier)) {
            n->destroy();
            ctx->rollback();
            return nullptr;
        }

        bool didCommit = false;
        if (n->isAsync) {
            // Safe to commit now, method declarations are the only rule that match the pattern:
            // ["public" | "private"], ["static"], "async", identifier
            ctx->commit();
            didCommit = true;
        }

        const Token* nameTok = ctx->get();
        n->extendLocation(nameTok);
        n->name = nameTok->toString();
        ctx->consume();
        
        n->typeParameters = TypeParameterListNode::TryParse(ctx);
        if (n->typeParameters) {
            if (n->typeParameters->isError()) {
                if (didCommit) {
                    n->m_isError = true;
                    if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) return n;
                } else {
                    n->destroy();
                    n->typeParameters->destroy();
                    ctx->rollback();
                    return nullptr;
                }
            }

            n->extendLocation(n->typeParameters);
        }

        n->parameters = ParameterListNode::TryParse(ctx);
        if (n->parameters) {
            if (n->parameters->isError()) {
                if (didCommit) {
                    n->m_isError = true;
                    if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_Colon)) return n;
                } else {
                    n->destroy();
                    n->parameters->destroy();
                    if (n->typeParameters) n->typeParameters->destroy();
                    ctx->rollback();
                    return nullptr;
                }
            }

            n->extendLocation(n->parameters);
        } else {
            if (didCommit) {
                ctx->logError("Expected parameter list");
                n->m_isError = true;
                if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_Colon)) return n;
            } else {
                n->destroy();
                if (n->typeParameters) n->typeParameters->destroy();
                ctx->rollback();
                return nullptr;
            }
        }
        
        if (!didCommit) {
            // Safe to commit now, method declarations are the only rule that match the pattern:
            // ["public" | "private"], ["static"], ["async"], identifier, "("
            ctx->commit();
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Colon)) {
            ctx->logError("Expected return type specifier");
            n->m_isError = true;
            return n;
        }

        ctx->consume(n);

        n->returnType = TypeSpecifierNode::TryParse(ctx);
        if (!n->returnType) {
            ctx->logError("Expected type specifier after ':'");
            n->m_isError = true;
            return n;
        }

        if (n->returnType->isError()) {
            n->returnType->destroy();
            n->returnType = nullptr;
            ctx->skipToAnyMatched({
                Match(TokenType::EndOfStatement),
                Match(TokenType::Symbol, TokenSubType::Symbol_OpenBrace)
            });
        } else {
            n->extendLocation(n->returnType);
        }

        if (ctx->match(TokenType::EndOfStatement)) {
            ctx->consume();
            return n;
        }

        n->body = StatementBlockNode::TryParse(ctx);
        if (!n->body) {
            ctx->logError("Expected function body");
            n->m_isError = true;
            return n;
        }

        n->extendLocation(n->body);
        return n;
    }
};
#include <parse/classes/ClassConstructorNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <parse/Context.h>
#include <utils/Array.hpp>

namespace parse {
    ClassConstructorNode::ClassConstructorNode(Context* ctx) : Node(ctx, NodeType::ClassConstructorNode), isPublic(true), body(nullptr) {}
    ClassConstructorNode::~ClassConstructorNode() {}
    void ClassConstructorNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ClassConstructorNode* ClassConstructorNode::Create(Context* ctx) { return new (ctx->allocNode()) ClassConstructorNode(ctx); }

    ClassConstructorNode* ClassConstructorNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        ctx->begin();

        ClassConstructorNode* n = Create(ctx);

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Private)) {
            ctx->consume(n);

            n->isPublic = false;
        } else if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Public)) {
            ctx->consume(n);

            n->isPublic = true;
        }
        
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Constructor)) {
            n->destroy();
            ctx->rollback();
            return nullptr;
        }

        ctx->commit();
        ctx->consume(n);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) {
            ctx->logError("Expected '('");
            n->m_isError = true;
            return n;
        }

        ctx->consume(n);

        TypedAssignableNode* param = TypedAssignableNode::TryParse(ctx);
        while (param) {
            if (param->isError()) {
                param->destroy();
                ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseParen);
                break;
            }

            n->parameters.push(param);
            n->extendLocation(param);

            if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Comma)) {
                ctx->consume(n);
                param = TypedAssignableNode::TryParse(ctx);

                if (!param) {
                    ctx->logError("Expected typed parameter after ','");
                    n->m_isError = true;
                    ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseParen);
                    break;
                }
            } else param = nullptr;
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseParen)) {
            ctx->logError("Expected ')'");
            n->m_isError = true;
            return n;
        }

        ctx->consume(n);

        if (ctx->match(TokenType::EndOfStatement)) {
            ctx->consume();
            return n;
        }

        n->body = StatementBlockNode::TryParse(ctx);
        if (!n->body) {
            ctx->logError("Expected function body or ';'");
            n->m_isError = true;
            return n;
        }

        n->extendLocation(n->body);
        return n;
    }
};
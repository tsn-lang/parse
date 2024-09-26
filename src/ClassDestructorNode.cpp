#include <parse/ClassDestructorNode.h>
#include <parse/Context.h>
#include <parse/StatementBlockNode.h>
#include <parse/TypedAssignableNode.h>
#include <utils/Array.hpp>

namespace parse {
    ClassDestructorNode::ClassDestructorNode(Context* ctx) : Node(ctx), isPublic(true), body(nullptr) {}
    void ClassDestructorNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ClassDestructorNode* ClassDestructorNode::Create(Context* ctx) { return new (ctx->allocNode()) ClassDestructorNode(ctx); }

    ClassDestructorNode* ClassDestructorNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        ctx->begin();

        ClassDestructorNode* n = Create(ctx);

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Private)) {
            n->extendLocation(ctx->get());
            ctx->consume();

            n->isPublic = false;
        } else if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Public)) {
            n->extendLocation(ctx->get());
            ctx->consume();

            n->isPublic = true;
        }
        
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Constructor)) {
            n->destroy();
            ctx->rollback();
            return nullptr;
        }

        ctx->commit();
        n->extendLocation(ctx->get());
        ctx->consume();

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) {
            ctx->logError("Expected '('");
            n->m_isError = true;
            return n;
        }

        n->extendLocation(ctx->get());
        ctx->consume();

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
                n->extendLocation(ctx->get());
                ctx->consume();
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

        n->extendLocation(ctx->get());
        ctx->consume();

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
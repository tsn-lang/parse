#include <parse/expressions/ExpressionSequenceNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/Context.h>
#include <utils/Array.hpp>

namespace parse {
    ExpressionSequenceNode::ExpressionSequenceNode(Context* ctx) : Node(ctx) {}
    ExpressionSequenceNode::~ExpressionSequenceNode() {}
    void ExpressionSequenceNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ExpressionSequenceNode* ExpressionSequenceNode::Create(Context* ctx) { return new (ctx->allocNode()) ExpressionSequenceNode(ctx); }

    ExpressionSequenceNode* ExpressionSequenceNode::TryParse(Context* ctx) {
        ExpressionNode* expr = ExpressionNode::TryParse(ctx);
        if (!expr) return nullptr;

        ExpressionSequenceNode* n = Create(ctx);

        while (expr) {
            n->expressions.push(expr);
            n->extendLocation(expr);

            if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Comma)) break;
            ctx->consume(n);

            expr = ExpressionNode::TryParse(ctx);
            if (!expr) {
                n->m_isError = true;
                ctx->logError("Expected expression");
                return n;
            }
        }

        return n;
    }

    ExpressionSequenceNode* ExpressionSequenceNode::TryParseParenthesized(Context* ctx) {
        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) return nullptr;

        ctx->begin();

        const Token* openTok = ctx->get();
        ctx->consume();

        ExpressionSequenceNode* n = TryParse(ctx);
        if (!n) {
            ctx->rollback();
            return nullptr;
        }

        n->extendLocation(openTok);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseParen)) {
            n->m_isError = true;
            ctx->logError("Expected ')'");
            return n;
        }

        ctx->consume(n);
        return n;
    }
};
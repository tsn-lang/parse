#include <parse/statements/WhileLoopNode.h>
#include <parse/proxies/StatementNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/Context.h>

namespace parse {
    WhileLoopNode::WhileLoopNode(Context* ctx) : Node(ctx), body(nullptr), condition(nullptr) {}
    void WhileLoopNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    WhileLoopNode* WhileLoopNode::Create(Context* ctx) { return new (ctx->allocNode()) WhileLoopNode(ctx); }

    WhileLoopNode* WhileLoopNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_While)) return nullptr;

        WhileLoopNode* n = Create(ctx);
        ctx->consume(n);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) {
            n->m_isError = true;
            ctx->logError("Expected '('");
            return n;
        }

        ctx->consume(n);

        n->condition = ExpressionNode::TryParse(ctx);
        if (!n->condition) {
            n->m_isError = true;
            ctx->logError("Expected expression");

            if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseParen)) return n;
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseParen)) {
            n->m_isError = true;
            ctx->logError("Expected ')'");
            return n;
        }

        ctx->consume(n);

        n->body = StatementNode::TryParse(ctx);
        if (!n->body) {
            n->m_isError = true;
            ctx->logError("Expected loop body");
            return n;
        }

        return n;
    }
};
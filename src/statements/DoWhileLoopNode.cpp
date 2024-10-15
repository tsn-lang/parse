#include <parse/statements/DoWhileLoopNode.h>
#include <parse/proxies/StatementNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/Context.h>

namespace parse {
    DoWhileLoopNode::DoWhileLoopNode(Context* ctx) : Node(ctx, NodeType::DoWhileLoopNode), body(nullptr), condition(nullptr) {}
    void DoWhileLoopNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    DoWhileLoopNode* DoWhileLoopNode::Create(Context* ctx) { return new (ctx->allocNode()) DoWhileLoopNode(ctx); }

    DoWhileLoopNode* DoWhileLoopNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Do)) return nullptr;

        DoWhileLoopNode* n = Create(ctx);
        ctx->consume(n);

        n->body = StatementNode::TryParse(ctx);
        if (!n->body) {
            n->m_isError = true;
            ctx->logError("Expected loop body");
            return n;
        }

        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_While)) {
            n->m_isError = true;
            ctx->logError("Expected 'while'");
            return n;
        }

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

        return n;
    }
};
#include <parse/statements/IfStatementNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/proxies/StatementNode.h>
#include <parse/Context.h>

namespace parse {
    IfStatementNode::IfStatementNode(Context* ctx) : Node(ctx, NodeType::IfStatementNode), condition(nullptr), body(nullptr), elseBody(nullptr) {}
    void IfStatementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    IfStatementNode* IfStatementNode::Create(Context* ctx) { return new (ctx->allocNode()) IfStatementNode(ctx); }

    IfStatementNode* IfStatementNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_If)) return nullptr;

        IfStatementNode* n = Create(ctx);
        ctx->consume(n);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) {
            ctx->logError("Expected '('");
            n->m_isError = true;
            ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseParen);
            return n;
        }

        ctx->consume(n);

        n->condition = ExpressionNode::TryParse(ctx);
        if (!n->condition) {
            ctx->logError("Expected expression");
            n->m_isError = true;
            if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseParen)) return n;
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseParen)) {
            ctx->logError("Expected ')'");
            n->m_isError = true;
            return n;
        }

        n->body = StatementNode::TryParse(ctx);
        if (!n->body) {
            ctx->logError("Expected if statement body");
            n->m_isError = true;
            return n;
        }

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Else)) {
            ctx->consume(n);

            n->elseBody = StatementNode::TryParse(ctx);
            if (!n->elseBody) {
                ctx->logError("Expected else statement body");
                n->m_isError = true;
                return n;
            }
        }

        return n;
    }
};
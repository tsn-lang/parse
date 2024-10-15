#include <parse/statements/ForLoopNode.h>
#include <parse/statements/DeclarationStatementNode.h>
#include <parse/proxies/StatementNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/Context.h>

namespace parse {
    ForLoopNode::ForLoopNode(Context* ctx)
        : Node(ctx, NodeType::ForLoopNode), initializer(nullptr), condition(nullptr), modifier(nullptr), body(nullptr)
    {}
    void ForLoopNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ForLoopNode* ForLoopNode::Create(Context* ctx) { return new (ctx->allocNode()) ForLoopNode(ctx); }

    ForLoopNode* ForLoopNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_For)) return nullptr;

        ForLoopNode* n = Create(ctx);
        ctx->consume(n);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) {
            ctx->logError("Expected '('");
            n->m_isError = true;
            ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseParen);
            return n;
        }

        ctx->consume(n);

        if (!ctx->match(TokenType::EndOfStatement)) {
            n->initializer = DeclarationStatementNode::TryParse(ctx);
            if (!n->initializer) {
                ctx->logError("Expected variable declaration");
                n->m_isError = true;
                if (!ctx->skipTo(TokenType::EndOfStatement)) return n;
            }

            if (!ctx->match(TokenType::EndOfStatement)) {
                ctx->logError("Expected ';'");
                n->m_isError = true;
                return n;
            }
        }

        ctx->consume(n);

        if (!ctx->match(TokenType::EndOfStatement)) {
            n->condition = ExpressionNode::TryParse(ctx);
            if (!n->condition) {
                ctx->logError("Expected expression");
                n->m_isError = true;
                if (!ctx->skipTo(TokenType::EndOfStatement)) return n;
            }

            if (!ctx->match(TokenType::EndOfStatement)) {
                ctx->logError("Expected ';'");
                n->m_isError = true;
                return n;
            }
        }

        ctx->consume(n);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseParen)) {
            n->modifier = ExpressionNode::TryParse(ctx);
            if (!n->condition) {
                ctx->logError("Expected expression");
                n->m_isError = true;
                if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseParen)) return n;
            }
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseParen)) {
            ctx->logError("Expected ')'");
            n->m_isError = true;
            return n;
        }

        n->body = StatementNode::TryParse(ctx);
        if (!n->body) {
            ctx->logError("Expected loop body");
            n->m_isError = true;
            return n;
        }

        return n;
    }
};
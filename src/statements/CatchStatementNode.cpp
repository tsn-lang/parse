#include <parse/statements/CatchStatementNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/proxies/StatementNode.h>
#include <parse/Context.h>

namespace parse {
    CatchStatementNode::CatchStatementNode(Context* ctx) : Node(ctx, NodeType::CatchStatementNode), exception(nullptr), body(nullptr) {}
    void CatchStatementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    CatchStatementNode* CatchStatementNode::Create(Context* ctx) { return new (ctx->allocNode()) CatchStatementNode(ctx); }

    CatchStatementNode* CatchStatementNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Catch)) return nullptr;

        CatchStatementNode* n = Create(ctx);
        ctx->consume(n);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) {
            ctx->logError("Expected '('");
            n->m_isError = true;
            return n;
        }

        ctx->consume(n);

        n->exception = TypedAssignableNode::TryParse(ctx);
        if (!n->exception) {
            ctx->logError("Expected typed parameter");
            n->m_isError = true;
            return n;
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseParen)) {
            ctx->logError("Expected ')'");
            n->m_isError = true;
            return n;
        }

        ctx->consume(n);

        n->body = StatementNode::TryParse(ctx);
        if (!n->body) {
            ctx->logError("Expected catch statement body");
            n->m_isError = true;
            return n;
        }

        return n;
    }
};
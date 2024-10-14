#include <parse/statements/DeleteStatementNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/Context.h>

namespace parse {
    DeleteStatementNode::DeleteStatementNode(Context* ctx) : Node(ctx) {}
    void DeleteStatementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    DeleteStatementNode* DeleteStatementNode::Create(Context* ctx) { return new (ctx->allocNode()) DeleteStatementNode(ctx); }

    DeleteStatementNode* DeleteStatementNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Delete)) return nullptr;

        DeleteStatementNode* n = Create(ctx);
        ctx->consume(n);

        n->expr = ExpressionNode::TryParse(ctx);
        if (!n->expr) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            ctx->skipTo(TokenType::EndOfStatement);
            return n;
        }

        return n;
    }
};
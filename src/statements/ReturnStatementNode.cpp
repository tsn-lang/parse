#include <parse/statements/ReturnStatementNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/Context.h>

namespace parse {
    ReturnStatementNode::ReturnStatementNode(Context* ctx) : Node(ctx, NodeType::ReturnStatementNode), returnValue(nullptr) {}
    void ReturnStatementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ReturnStatementNode* ReturnStatementNode::Create(Context* ctx) { return new (ctx->allocNode()) ReturnStatementNode(ctx); }

    ReturnStatementNode* ReturnStatementNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Return)) return nullptr;

        ReturnStatementNode* n = Create(ctx);
        ctx->consume(n);

        if (ctx->match(TokenType::EndOfStatement)) return n;

        n->returnValue = ExpressionNode::TryParse(ctx);
        if (!n) {
            ctx->logError("Expected expression or ';'");
            n->m_isError = true;
            ctx->skipTo(TokenType::EndOfStatement);
            return n;
        }

        n->extendLocation(n->returnValue);

        return n;
    }
};
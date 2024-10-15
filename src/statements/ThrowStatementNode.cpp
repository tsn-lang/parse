#include <parse/statements/ThrowStatementNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/Context.h>

namespace parse {
    ThrowStatementNode::ThrowStatementNode(Context* ctx) : Node(ctx, NodeType::ThrowStatementNode), value(nullptr) {}
    void ThrowStatementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ThrowStatementNode* ThrowStatementNode::Create(Context* ctx) { return new (ctx->allocNode()) ThrowStatementNode(ctx); }

    ThrowStatementNode* ThrowStatementNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Throw)) return nullptr;

        ThrowStatementNode* n = Create(ctx);
        ctx->consume(n);

        n->value = ExpressionNode::TryParse(ctx);
        if (!n) {
            ctx->logError("Expected expression");
            n->m_isError = true;
            ctx->skipTo(TokenType::EndOfStatement);
            return n;
        }

        n->extendLocation(n->value);
        return n;
    }
};
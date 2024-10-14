#include <parse/expressions/ThisExpressionNode.h>
#include <parse/Context.h>

namespace parse {
    ThisExpressionNode::ThisExpressionNode(Context* ctx) : Node(ctx) {}
    void ThisExpressionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ThisExpressionNode* ThisExpressionNode::Create(Context* ctx) { return new (ctx->allocNode()) ThisExpressionNode(ctx); }

    ThisExpressionNode* ThisExpressionNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        return nullptr;
    }
};
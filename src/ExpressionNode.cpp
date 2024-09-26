#include <parse/ExpressionNode.h>
#include <parse/Context.h>

namespace parse {
    ExpressionNode::ExpressionNode(Context* ctx) : Node(ctx) {}
    void ExpressionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ExpressionNode* ExpressionNode::Create(Context* ctx) { return new (ctx->allocNode()) ExpressionNode(ctx); }

    ExpressionNode* ExpressionNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        return nullptr;
    }
};
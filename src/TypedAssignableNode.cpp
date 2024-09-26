#include <parse/TypedAssignableNode.h>
#include <parse/Context.h>

namespace parse {
    TypedAssignableNode::TypedAssignableNode(Context* ctx) : Node(ctx) {}
    void TypedAssignableNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    TypedAssignableNode* TypedAssignableNode::Create(Context* ctx) { return new (ctx->allocNode()) TypedAssignableNode(ctx); }

    TypedAssignableNode* TypedAssignableNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        return nullptr;
    }
};
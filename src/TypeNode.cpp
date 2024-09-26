#include <parse/TypeNode.h>
#include <parse/Context.h>

namespace parse {
    TypeNode::TypeNode(Context* ctx) : Node(ctx) {}
    void TypeNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    TypeNode* TypeNode::Create(Context* ctx) { return new (ctx->allocNode()) TypeNode(ctx); }

    TypeNode* TypeNode::TryParse(Context* ctx) {
        return nullptr;
    }
};
#include <parse/ObjectTypeNode.h>
#include <parse/Context.h>

namespace parse {
    ObjectTypeNode::ObjectTypeNode(Context* ctx) : Node(ctx) {}
    void ObjectTypeNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ObjectTypeNode* ObjectTypeNode::Create(Context* ctx) { return new (ctx->allocNode()) ObjectTypeNode(ctx); }

    ObjectTypeNode* ObjectTypeNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        return nullptr;
    }
};
#include <parse/ObjectTypePropertyNode.h>
#include <parse/Context.h>

namespace parse {
    ObjectTypePropertyNode::ObjectTypePropertyNode(Context* ctx) : Node(ctx) {}
    void ObjectTypePropertyNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ObjectTypePropertyNode* ObjectTypePropertyNode::Create(Context* ctx) { return new (ctx->allocNode()) ObjectTypePropertyNode(ctx); }

    ObjectTypePropertyNode* ObjectTypePropertyNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        return nullptr;
    }
};
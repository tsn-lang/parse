#include <parse/ArrayTypeNode.h>
#include <parse/Context.h>

namespace parse {
    ArrayTypeNode::ArrayTypeNode(Context* ctx) : Node(ctx) {}
    void ArrayTypeNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ArrayTypeNode* ArrayTypeNode::Create(Context* ctx) { return new (ctx->allocNode()) ArrayTypeNode(ctx); }

    ArrayTypeNode* ArrayTypeNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        return nullptr;
    }
};
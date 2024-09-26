#include <parse/ArrayTypeElementNode.h>
#include <parse/Context.h>

namespace parse {
    ArrayTypeElementNode::ArrayTypeElementNode(Context* ctx) : Node(ctx) {}
    void ArrayTypeElementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ArrayTypeElementNode* ArrayTypeElementNode::Create(Context* ctx) { return new (ctx->allocNode()) ArrayTypeElementNode(ctx); }

    ArrayTypeElementNode* ArrayTypeElementNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        return nullptr;
    }
};
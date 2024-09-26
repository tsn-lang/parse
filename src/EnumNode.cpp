#include <parse/EnumNode.h>
#include <parse/Context.h>

namespace parse {
    EnumNode::EnumNode(Context* ctx) : Node(ctx) {}
    void EnumNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    EnumNode* EnumNode::Create(Context* ctx) { return new (ctx->allocNode()) EnumNode(ctx); }

    EnumNode* EnumNode::TryParse(Context* ctx) {
        return nullptr;
    }
};
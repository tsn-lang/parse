#include <parse/literals/NullLiteralNode.h>
#include <parse/Context.h>

namespace parse {
    NullLiteralNode::NullLiteralNode(Context* ctx) : Node(ctx) {}
    void NullLiteralNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    NullLiteralNode* NullLiteralNode::Create(Context* ctx) { return new (ctx->allocNode()) NullLiteralNode(ctx); }
};
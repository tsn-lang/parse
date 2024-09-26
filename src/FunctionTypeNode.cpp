#include <parse/FunctionTypeNode.h>
#include <parse/Context.h>

namespace parse {
    FunctionTypeNode::FunctionTypeNode(Context* ctx) : Node(ctx) {}
    void FunctionTypeNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    FunctionTypeNode* FunctionTypeNode::Create(Context* ctx) { return new (ctx->allocNode()) FunctionTypeNode(ctx); }

    FunctionTypeNode* FunctionTypeNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        return nullptr;
    }
};
#include <parse/AliasTypeNode.h>
#include <parse/Context.h>

namespace parse {
    AliasTypeNode::AliasTypeNode(Context* ctx) : Node(ctx) {}
    void AliasTypeNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    AliasTypeNode* AliasTypeNode::Create(Context* ctx) { return new (ctx->allocNode()) AliasTypeNode(ctx); }

    AliasTypeNode* AliasTypeNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        return nullptr;
    }
};
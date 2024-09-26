#include <parse/TypeSpecifierNode.h>
#include <parse/Context.h>

namespace parse {
    TypeSpecifierNode::TypeSpecifierNode(Context* ctx) : Node(ctx) {}
    void TypeSpecifierNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    TypeSpecifierNode* TypeSpecifierNode::Create(Context* ctx) { return new (ctx->allocNode()) TypeSpecifierNode(ctx); }

    TypeSpecifierNode* TypeSpecifierNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        return nullptr;
    }
};
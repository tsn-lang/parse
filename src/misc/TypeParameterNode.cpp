#include <parse/misc/TypeParameterNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    TypeParameterNode::TypeParameterNode(Context* ctx) : Node(ctx, NodeType::TypeParameterNode) {}
    void TypeParameterNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    TypeParameterNode* TypeParameterNode::Create(Context* ctx) { return new (ctx->allocNode()) TypeParameterNode(ctx); }

    TypeParameterNode* TypeParameterNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Identifier)) return nullptr;
        
        TypeParameterNode* n = Create(ctx);
        n->name = ctx->get()->toString();
        ctx->consume(n);

        return n;
    }
};
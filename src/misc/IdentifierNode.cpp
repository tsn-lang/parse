#include <parse/misc/IdentifierNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    IdentifierNode::IdentifierNode(Context* ctx) : Node(ctx, NodeType::IdentifierNode) {}
    IdentifierNode::~IdentifierNode() {}
    void IdentifierNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    IdentifierNode* IdentifierNode::Create(Context* ctx) { return new (ctx->allocNode()) IdentifierNode(ctx); }

    IdentifierNode* IdentifierNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Identifier)) return nullptr;

        IdentifierNode* n = Create(ctx);
        n->text = ctx->get()->toString();
        ctx->consume(n);
        
        return n;
    }
};
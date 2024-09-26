#include <parse/IdentifierNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    IdentifierNode::IdentifierNode(Context* ctx) : Node(ctx) {}
    void IdentifierNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    IdentifierNode* IdentifierNode::Create(Context* ctx) { return new (ctx->allocNode()) IdentifierNode(ctx); }

    IdentifierNode* IdentifierNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Identifier)) return nullptr;

        const Token* tok = ctx->get();
        ctx->consume();

        IdentifierNode* n = Create(ctx);
        n->extendLocation(tok);
        n->text = tok->toString();
        
        return n;
    }
};
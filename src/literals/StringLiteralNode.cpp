#include <parse/literals/StringLiteralNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    StringLiteralNode::StringLiteralNode(Context* ctx) : Node(ctx) {}
    StringLiteralNode::~StringLiteralNode() {}
    void StringLiteralNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    StringLiteralNode* StringLiteralNode::Create(Context* ctx) { return new (ctx->allocNode()) StringLiteralNode(ctx); }

    StringLiteralNode* StringLiteralNode::TryParse(Context* ctx) {
        if (ctx->match(TokenType::Literal, TokenSubType::Literal_String)) return nullptr;

        const Token* tok = ctx->get();

        StringLiteralNode* n = Create(ctx);
        n->value = tok->getContentString();
        n->extendLocation(tok);
        
        ctx->consume();

        return n;
    }
};
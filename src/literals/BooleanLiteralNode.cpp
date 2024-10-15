#include <parse/literals/BooleanLiteralNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    BooleanLiteralNode::BooleanLiteralNode(Context* ctx) : Node(ctx, NodeType::BooleanLiteralNode), value(false) {}
    void BooleanLiteralNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    BooleanLiteralNode* BooleanLiteralNode::Create(Context* ctx) { return new (ctx->allocNode()) BooleanLiteralNode(ctx); }

    BooleanLiteralNode* BooleanLiteralNode::TryParse(Context* ctx) {
        const Token* tok = ctx->get();
        if (!tok || tok->type != TokenType::Literal) return nullptr;

        if (tok->subType == i32(TokenSubType::Literal_True)) {
            const Token* tok = ctx->get();

            BooleanLiteralNode* n = Create(ctx);
            n->value = true;
            n->extendLocation(tok);
            
            ctx->consume();
            return n;
        } else if (tok->subType == i32(TokenSubType::Literal_False)) {
            const Token* tok = ctx->get();

            BooleanLiteralNode* n = Create(ctx);
            n->value = false;
            n->extendLocation(tok);
            
            ctx->consume();
            return n;
        }

        return nullptr;
    }
};
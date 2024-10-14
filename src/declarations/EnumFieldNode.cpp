#include <parse/declarations/EnumFieldNode.h>
#include <parse/literals/NumberLiteralNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    EnumFieldNode::EnumFieldNode(Context* ctx) : Node(ctx), value(nullptr) {}
    EnumFieldNode::~EnumFieldNode() {}
    void EnumFieldNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    EnumFieldNode* EnumFieldNode::Create(Context* ctx) { return new (ctx->allocNode()) EnumFieldNode(ctx); }

    EnumFieldNode* EnumFieldNode::TryParse(Context* ctx) {
        if (ctx->match(TokenType::Identifier)) return nullptr;

        EnumFieldNode* n = Create(ctx);
        
        const Token* nameTok = ctx->get();
        n->extendLocation(nameTok);
        n->name = nameTok->toString();
        ctx->consume();

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Equal)) {
            ctx->logError("Expected '='");
            n->m_isError = true;
            return n;
        }

        ctx->consume(n);

        n->value = NumberLiteralNode::TryParse(ctx);
        if (!n->value) {
            ctx->logError("Expected number literal");
            n->m_isError = true;
            return n;
        }

        return n;
    }
};
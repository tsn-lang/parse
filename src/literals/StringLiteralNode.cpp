#include <parse/literals/StringLiteralNode.h>
#include <parse/Context.h>
#include <parse/Tokens.h>
#include <tokenize/Token.h>

namespace parse {
    StringLiteralNode::StringLiteralNode(Context* ctx) : Node(ctx, NodeType::StringLiteralNode) {}
    StringLiteralNode::~StringLiteralNode() {}
    void StringLiteralNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    StringLiteralNode* StringLiteralNode::Create(Context* ctx) { return new (ctx->allocNode()) StringLiteralNode(ctx); }

    StringLiteralNode* StringLiteralNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Literal, TokenSubType::Literal_String)) return nullptr;

        const Token* tok = ctx->get();

        StringLiteralNode* n = Create(ctx);

        String content = tok->getContentString();
        for (u32 i = 1;i < content.size() - 1;i++) {
            if (content[i] == '\\') {
                i += parseEscapeSequence(&content[i + 1], n->value, ctx);
                continue;
            }

            n->value += content[i];
        }

        n->extendLocation(tok);
        ctx->consume();

        return n;
    }
};
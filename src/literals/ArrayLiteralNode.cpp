#include <parse/literals/ArrayLiteralNode.h>
#include <parse/expressions/ExpressionSequenceNode.h>
#include <parse/Context.h>

namespace parse {
    ArrayLiteralNode::ArrayLiteralNode(Context* ctx) : Node(ctx, NodeType::ArrayLiteralNode), elements(nullptr) {}
    void ArrayLiteralNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ArrayLiteralNode* ArrayLiteralNode::Create(Context* ctx) { return new (ctx->allocNode()) ArrayLiteralNode(ctx); }

    ArrayLiteralNode* ArrayLiteralNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenBracket)) return nullptr;

        ArrayLiteralNode* n = Create(ctx);
        ctx->consume(n);

        n->elements = ExpressionSequenceNode::TryParse(ctx);
        if (n->elements) n->extendLocation(n->elements);
        
        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseBracket)) {
            n->m_isError = true;
            if (n->elements) ctx->logError("Expected expression sequence or ']'");
            else ctx->logError("Expected ']'");
            if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseBracket)) return n;
        }

        ctx->consume(n);
        return n;
    }
};
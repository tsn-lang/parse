#include <parse/expressions/CastExpressionNode.h>
#include <parse/expressions/UnaryExpressionNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/Context.h>

namespace parse {
    CastExpressionNode::CastExpressionNode(Context* ctx) : Node(ctx), expr(nullptr), asType(nullptr) {}
    void CastExpressionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    CastExpressionNode* CastExpressionNode::Create(Context* ctx) { return new (ctx->allocNode()) CastExpressionNode(ctx); }

    Node* CastExpressionNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* expr = UnaryExpressionNode::TryParsePrefixUnaryExpression(ctx);
        if (!expr) return nullptr;

        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_As)) return expr;

        CastExpressionNode* n = Create(ctx);
        n->expr = expr;
        n->extendLocation(n->expr);
        n->m_isError = expr->isError();
        ctx->consume(n);

        n->asType = TypeSpecifierNode::TryParse(ctx);
        if (!n->asType) {
            n->m_isError = true;
            ctx->logError("Expected type specifier");
            return n;
        }

        n->extendLocation(n->asType);
        n->m_isError = n->m_isError || n->asType->isError();
        return n;
    }
};
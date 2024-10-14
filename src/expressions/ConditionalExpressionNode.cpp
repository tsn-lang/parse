#include <parse/expressions/ConditionalExpressionNode.h>
#include <parse/expressions/BinaryExpressionNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/Context.h>

namespace parse {
    ConditionalExpressionNode::ConditionalExpressionNode(Context* ctx)
        : Node(ctx), condition(nullptr), valueOnTrue(nullptr), valueOnFalse(nullptr)
    {}
    void ConditionalExpressionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ConditionalExpressionNode* ConditionalExpressionNode::Create(Context* ctx) { return new (ctx->allocNode()) ConditionalExpressionNode(ctx); }

    Node* ConditionalExpressionNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* expr = BinaryExpressionNode::TryParseLogicalOrExpression(ctx);
        if (!expr) return nullptr;

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_QuestionMark)) return expr;

        ConditionalExpressionNode* n = Create(ctx);
        n->condition = expr;
        n->m_isError = n->condition->isError();
        n->extendLocation(n->condition);
        ctx->consume(n);

        n->valueOnTrue = ExpressionNode::TryParse(ctx);
        if (!n->valueOnTrue) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            return n;
        }
        n->m_isError = n->m_isError || n->valueOnTrue->isError();
        n->extendLocation(n->valueOnTrue);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Colon)) {
            n->m_isError = true;
            ctx->logError("Expected ':'");
            return n;
        }

        ctx->consume(n);

        n->valueOnFalse = ExpressionNode::TryParse(ctx);
        if (!n->valueOnFalse) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            return n;
        }
        n->m_isError = n->m_isError || n->valueOnFalse->isError();
        n->extendLocation(n->valueOnFalse);

        return n;
    }
};
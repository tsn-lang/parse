#include <parse/expressions/AwaitExpressionNode.h>
#include <parse/expressions/BinaryExpressionNode.h>
#include <parse/Context.h>

namespace parse {
    AwaitExpressionNode::AwaitExpressionNode(Context* ctx) : Node(ctx, NodeType::AwaitExpressionNode), expr(nullptr) {}
    void AwaitExpressionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    AwaitExpressionNode* AwaitExpressionNode::Create(Context* ctx) { return new (ctx->allocNode()) AwaitExpressionNode(ctx); }

    Node* AwaitExpressionNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Await)) {
            return BinaryExpressionNode::TryParseAssignmentOperatorExpression(ctx);
        }

        AwaitExpressionNode* n = Create(ctx);
        n->expr = BinaryExpressionNode::TryParseAssignmentOperatorExpression(ctx);

        if (!n->expr) {
            n->m_isError = true;
            return n;
        }

        n->extendLocation(n->expr);

        return n;
    }
};
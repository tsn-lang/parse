#include <parse/expressions/IndexExpressionNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/expressions/FunctionExpressionNode.h>
#include <parse/expressions/ArrowFunctionExpressionNode.h>
#include <parse/expressions/MemberExpressionNode.h>
#include <parse/Context.h>

namespace parse {
    IndexExpressionNode::IndexExpressionNode(Context* ctx) : Node(ctx, NodeType::IndexExpressionNode), object(nullptr), index(nullptr) {}
    void IndexExpressionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    IndexExpressionNode* IndexExpressionNode::Create(Context* ctx) { return new (ctx->allocNode()) IndexExpressionNode(ctx); }

    Node* IndexExpressionNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* base = ExpressionNode::TryParsePrimaryExpression(ctx);
        if (!base) base = FunctionExpressionNode::TryParse(ctx);
        if (!base) base = ArrowFunctionExpressionNode::TryParse(ctx);
        if (!base) return nullptr;

        Node* n = base;
        while (n) {
            IndexExpressionNode* index = TryParseTail(ctx, n);
            if (index) n = index;

            MemberExpressionNode* member = MemberExpressionNode::TryParseTail(ctx, n);
            if (member) n = member;

            if (!index && !member) break;
        }

        return n;
    }

    IndexExpressionNode* IndexExpressionNode::TryParseTail(Context* ctx, Node* base) {
        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenBracket)) return nullptr;
        
        IndexExpressionNode* n = Create(ctx);
        ctx->consume(n);

        n->object = base;
        n->extendLocation(base);
        n->m_isError = base->isError();
        
        n->index = ExpressionNode::TryParse(ctx);
        if (!n->index) {
            n->m_isError = true;
            ctx->logError("Expected expression");

            if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseBracket)) {
                return n;
            }
        }

        n->extendLocation(n->index);
        n->m_isError = n->m_isError || n->index->isError();

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseBracket)) {
            n->m_isError = true;
            ctx->logError("Expected ']'");
            return n;
        }

        ctx->consume(n);
        return n;
    }
};
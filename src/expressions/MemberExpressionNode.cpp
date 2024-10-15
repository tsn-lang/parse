#include <parse/expressions/MemberExpressionNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/expressions/FunctionExpressionNode.h>
#include <parse/expressions/ArrowFunctionExpressionNode.h>
#include <parse/expressions/IndexExpressionNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    MemberExpressionNode::MemberExpressionNode(Context* ctx) : Node(ctx, NodeType::MemberExpressionNode), object(nullptr) {}
    MemberExpressionNode::~MemberExpressionNode() {}
    void MemberExpressionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    MemberExpressionNode* MemberExpressionNode::Create(Context* ctx) { return new (ctx->allocNode()) MemberExpressionNode(ctx); }

    Node* MemberExpressionNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* base = ExpressionNode::TryParsePrimaryExpression(ctx);
        if (!base) base = FunctionExpressionNode::TryParse(ctx);
        if (!base) base = ArrowFunctionExpressionNode::TryParse(ctx);
        if (!base) return nullptr;

        Node* n = base;
        while (n) {
            MemberExpressionNode* member = TryParseTail(ctx, n);
            if (member) n = member;

            IndexExpressionNode* index = IndexExpressionNode::TryParseTail(ctx, n);
            if (index) n = index;

            if (!member && !index) break;
        }

        return n;
    }

    MemberExpressionNode* MemberExpressionNode::TryParseTail(Context* ctx, Node* base) {
        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Dot)) return nullptr;
        
        MemberExpressionNode* n = Create(ctx);
        ctx->consume(n);

        n->object = base;
        n->extendLocation(base);
        n->m_isError = base->isError();
        
        if (!ctx->match(TokenType::Identifier)) {
            n->m_isError = true;
            ctx->logError("Expected identifier");
            return n;
        }

        n->propertyName = ctx->get()->toString();
        ctx->consume(n);
        return n;
    }
};
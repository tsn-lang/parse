#include <parse/expressions/CallExpressionNode.h>
#include <parse/expressions/NewExpressionNode.h>
#include <parse/expressions/ExpressionSequenceNode.h>
#include <parse/expressions/MemberExpressionNode.h>
#include <parse/expressions/IndexExpressionNode.h>
#include <parse/Context.h>

namespace parse {
    CallExpressionNode::CallExpressionNode(Context* ctx) : Node(ctx, NodeType::CallExpressionNode), callee(nullptr), arguments(nullptr) {}
    void CallExpressionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    CallExpressionNode* CallExpressionNode::Create(Context* ctx) { return new (ctx->allocNode()) CallExpressionNode(ctx); }

    Node* CallExpressionNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* base = NewExpressionNode::TryParse(ctx);
        if (!base) return nullptr;

        CallExpressionNode* n = TryParseTail(ctx, base);
        if (!n) return base;

        Node* last = n;
        while (last) {
            IndexExpressionNode* idx = IndexExpressionNode::TryParseTail(ctx, last);
            if (idx) last = idx;

            MemberExpressionNode* member = MemberExpressionNode::TryParseTail(ctx, last);
            if (member) last = member;

            CallExpressionNode* call = CallExpressionNode::TryParseTail(ctx, last);
            if (call) last = call;

            if (!idx && !member && !call) break;
        }

        return last;
    }
    
    CallExpressionNode* CallExpressionNode::TryParseTail(Context* ctx, Node* base) {
        if (!base || ctx->atEnd()) return nullptr;

        ExpressionSequenceNode* args = ExpressionSequenceNode::TryParseParenthesized(ctx);
        if (!args) return nullptr;

        CallExpressionNode* n = Create(ctx);
        n->callee = base;
        n->m_isError = base->isError();
        n->extendLocation(n->callee);

        n->arguments = args;
        n->m_isError = n->m_isError || n->arguments->isError();
        n->extendLocation(n->arguments);

        return n;
    }
};
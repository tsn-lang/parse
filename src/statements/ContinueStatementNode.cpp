#include <parse/statements/ContinueStatementNode.h>
#include <parse/Context.h>

namespace parse {
    ContinueStatementNode::ContinueStatementNode(Context* ctx) : Node(ctx, NodeType::ContinueStatementNode) {}
    void ContinueStatementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ContinueStatementNode* ContinueStatementNode::Create(Context* ctx) { return new (ctx->allocNode()) ContinueStatementNode(ctx); }
};
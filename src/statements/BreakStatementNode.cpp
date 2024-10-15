#include <parse/statements/BreakStatementNode.h>
#include <parse/Context.h>

namespace parse {
    BreakStatementNode::BreakStatementNode(Context* ctx) : Node(ctx, NodeType::BreakStatementNode) {}
    void BreakStatementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    BreakStatementNode* BreakStatementNode::Create(Context* ctx) { return new (ctx->allocNode()) BreakStatementNode(ctx); }
};
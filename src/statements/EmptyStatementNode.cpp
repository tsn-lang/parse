#include <parse/statements/EmptyStatementNode.h>
#include <parse/Context.h>

namespace parse {
    EmptyStatementNode::EmptyStatementNode(Context* ctx) : Node(ctx) {}
    void EmptyStatementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    EmptyStatementNode* EmptyStatementNode::Create(Context* ctx) { return new (ctx->allocNode()) EmptyStatementNode(ctx); }
};
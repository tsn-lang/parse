#include <parse/StatementNode.h>
#include <parse/Context.h>

namespace parse {
    StatementNode::StatementNode(Context* ctx) : Node(ctx) {}
    void StatementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    StatementNode* StatementNode::Create(Context* ctx) { return new (ctx->allocNode()) StatementNode(ctx); }

    StatementNode* StatementNode::TryParse(Context* ctx) {
        return nullptr;
    }
};
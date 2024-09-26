#include <parse/StatementBlockNode.h>
#include <parse/Context.h>

namespace parse {
    StatementBlockNode::StatementBlockNode(Context* ctx) : Node(ctx) {}
    void StatementBlockNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    StatementBlockNode* StatementBlockNode::Create(Context* ctx) { return new (ctx->allocNode()) StatementBlockNode(ctx); }

    StatementBlockNode* StatementBlockNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        return nullptr;
    }
};
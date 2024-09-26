#include <parse/ErrorNode.h>
#include <parse/Context.h>

namespace parse {
    ErrorNode::ErrorNode(Context* ctx) : Node(ctx) {
        m_isError = true;
    }

    void ErrorNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ErrorNode* ErrorNode::Create(Context* ctx) { return new (ctx->allocNode()) ErrorNode(ctx); }
};
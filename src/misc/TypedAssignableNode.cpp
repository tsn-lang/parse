#include <parse/misc/TypedAssignableNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    TypedAssignableNode::TypedAssignableNode(Context* ctx) : Node(ctx, NodeType::TypedAssignableNode), type(nullptr) {}
    TypedAssignableNode::~TypedAssignableNode() {}
    void TypedAssignableNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    TypedAssignableNode* TypedAssignableNode::Create(Context* ctx) { return new (ctx->allocNode()) TypedAssignableNode(ctx); }

    TypedAssignableNode* TypedAssignableNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Identifier)) return nullptr;

        ctx->begin();
        TypedAssignableNode* n = Create(ctx);
        n->name = ctx->get()->toString();
        ctx->consume(n);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Colon)) {
            n->destroy();
            ctx->rollback();
            return nullptr;
        }

        ctx->commit();
        ctx->consume(n);

        n->type = TypeSpecifierNode::TryParse(ctx);
        if (!n->type) {
            n->m_isError = true;
            ctx->logError("Expected type specifier");
            return n;
        }

        return n;
    }
};
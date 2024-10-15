#include <parse/declarations/TypeNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/misc/TypeParameterListNode.h>
#include <parse/Context.h>

namespace parse {
    TypeNode::TypeNode(Context* ctx) : Node(ctx, NodeType::TypeNode), type(nullptr), parameters(nullptr) {}
    TypeNode::~TypeNode() {}
    void TypeNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    TypeNode* TypeNode::Create(Context* ctx) { return new (ctx->allocNode()) TypeNode(ctx); }

    TypeNode* TypeNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Type)) return nullptr;

        TypeNode* n = Create(ctx);
        ctx->consume(n);

        if (!ctx->match(TokenType::Identifier)) {
            ctx->logError("Expected identifier");
            n->m_isError = true;

            if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_Equal)) {
                return n;
            }
        }

        n->parameters = TypeParameterListNode::TryParse(ctx);
        if (n->parameters && n->parameters->isError()) {
            n->m_isError = true;

            if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_Equal)) {
                return n;
            }
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Equal)) {
            ctx->logError("Expected '='");
            n->m_isError = true;
            return n;
        }

        ctx->consume(n);

        n->type = TypeSpecifierNode::TryParse(ctx);
        if (!n->type) {
            ctx->logError("Expected type specifier");
            n->m_isError = true;
            return n;
        }

        n->extendLocation(n->type);
        return n;
    }
};
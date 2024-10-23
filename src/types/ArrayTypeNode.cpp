#include <parse/types/ArrayTypeNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <utils/Array.hpp>
#include <parse/Context.h>

namespace parse {
    ArrayTypeNode::ArrayTypeNode(Context* ctx) : Node(ctx, NodeType::ArrayTypeNode) {}
    ArrayTypeNode::~ArrayTypeNode() {}
    void ArrayTypeNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ArrayTypeNode* ArrayTypeNode::Create(Context* ctx) { return new (ctx->allocNode()) ArrayTypeNode(ctx); }

    ArrayTypeNode* ArrayTypeNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenBracket)) return nullptr;

        ctx->begin();
        ArrayTypeNode* n = Create(ctx);
        ctx->consume(n);

        TypedAssignableNode* elem = TypedAssignableNode::TryParse(ctx);
        if (!elem) {
            ctx->rollback();
            n->destroy();
            return nullptr;
        }
        
        ctx->commit();

        while (elem) {
            if (elem->isError()) {
                n->m_isError = true;
                elem->destroy();
                if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseBracket)) return n;
                break;
            }

            n->elements.push(elem);
            n->extendLocation(elem);

            if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Comma)) {
                ctx->consume(n);
                elem = TypedAssignableNode::TryParse(ctx);

                if (!elem) {
                    ctx->logError("Expected typed assignable");
                    n->m_isError = true;
                    if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseBracket)) return n;
                    break;
                }
            } else elem = nullptr;
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseBracket)) {
            ctx->logError("Expected ']'");
            n->m_isError = true;
        } else {
            ctx->consume(n);
        }

        return n;
    }
};
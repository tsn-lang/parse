#include <parse/types/ObjectTypeNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/Context.h>

#include <utils/Array.hpp>

namespace parse {
    ObjectTypeNode::ObjectTypeNode(Context* ctx) : Node(ctx, NodeType::ObjectTypeNode) {}
    ObjectTypeNode::~ObjectTypeNode() {}
    void ObjectTypeNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ObjectTypeNode* ObjectTypeNode::Create(Context* ctx) { return new (ctx->allocNode()) ObjectTypeNode(ctx); }

    ObjectTypeNode* ObjectTypeNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenBrace)) return nullptr;

        ctx->begin();
        ObjectTypeNode* n = Create(ctx);
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
                elem->destroy();
                ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseBrace);
                break;
            }

            n->properties.push(elem);
            n->extendLocation(elem);

            if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Comma)) {
                ctx->consume(n);
                elem = TypedAssignableNode::TryParse(ctx);

                if (!elem) {
                    ctx->logError("Expected typed assignable after ','");
                    n->m_isError = true;
                    ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseBrace);
                    break;
                }
            } else elem = nullptr;
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)) {
            ctx->logError("Expected '}'");
            n->m_isError = true;
        } else {
            ctx->consume(n);
        }

        return n;
    }
};
#include <parse/literals/ObjectLiteralNode.h>
#include <parse/literals/ObjectLiteralPropertyNode.h>
#include <parse/Context.h>
#include <utils/Array.hpp>

namespace parse {
    ObjectLiteralNode::ObjectLiteralNode(Context* ctx) : Node(ctx, NodeType::ObjectLiteralNode) {}
    ObjectLiteralNode::~ObjectLiteralNode() {}
    void ObjectLiteralNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ObjectLiteralNode* ObjectLiteralNode::Create(Context* ctx) { return new (ctx->allocNode()) ObjectLiteralNode(ctx); }

    ObjectLiteralNode* ObjectLiteralNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenBrace)) return nullptr;

        ObjectLiteralNode* n = Create(ctx);
        ctx->begin();
        ctx->consume(n);

        ObjectLiteralPropertyNode* prop = ObjectLiteralPropertyNode::TryParse(ctx);
        if (!prop) {
            if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)) {
                ctx->commit();
                ctx->consume(n);
                return n;
            }

            ctx->rollback();
            n->destroy();
            return nullptr;
        }

        ctx->commit();

        if (prop->isError()) {
            n->m_isError = true;
            
            if (!ctx->skipToAnyMatched({
                Match(TokenType::Symbol, TokenSubType::Symbol_Comma),
                Match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)
            })) {
                return n;
            }
        }

        while (prop) {
            n->properties.push(prop);
            n->extendLocation(prop);

            if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Comma)) break;
            ctx->consume(n);

            prop = ObjectLiteralPropertyNode::TryParse(ctx);
            if (!prop) {
                n->m_isError = true;
                ctx->logError("Expected object property");
                return n;
            }
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)) {
            n->m_isError = true;
            ctx->logError("Expected '}'");
            if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)) return n;
        }

        ctx->consume(n);
        return n;
    }
};
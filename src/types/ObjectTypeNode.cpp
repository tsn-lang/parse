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

        while (true) {
            TypedAssignableNode* elem = TypedAssignableNode::TryParse(ctx);
            if (!elem) break;

            if (elem->isError()) {
                n->m_isError = true;
                elem->destroy();
                if (!ctx->skipToAnyMatched({
                    Match(TokenType::EndOfStatement),
                    Match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)
                })) return n;
            }
            
            n->properties.push(elem);
            n->extendLocation(elem);

            if (ctx->match(TokenType::EndOfStatement)) ctx->consume(n);
            else {
                n->m_isError = true;
                ctx->logError("Expected ';'");
                if (!ctx->skipToAnyMatched({
                    Match(TokenType::EndOfStatement),
                    Match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)
                })) return n;

                if (ctx->match(TokenType::EndOfStatement)) ctx->consume(n);
                else break;
            }
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
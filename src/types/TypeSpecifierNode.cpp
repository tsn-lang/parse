#include <parse/types/TypeSpecifierNode.h>
#include <parse/types/IdentifierTypeSpecifierNode.h>
#include <parse/types/ArrayTypeNode.h>
#include <parse/types/ObjectTypeNode.h>
#include <parse/types/FunctionTypeNode.h>
#include <parse/Context.h>

namespace parse {
    TypeSpecifierNode::TypeSpecifierNode(Context* ctx) : Node(ctx, NodeType::TypeSpecifierNode), type(nullptr), mod(Modifier::None) {}
    void TypeSpecifierNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    TypeSpecifierNode* TypeSpecifierNode::Create(Context* ctx) { return new (ctx->allocNode()) TypeSpecifierNode(ctx); }

    TypeSpecifierNode* TypeSpecifierNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        ctx->begin();

        TypeSpecifierNode* n = Create(ctx);

        bool isParenthesized = ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen);
        if (isParenthesized) {
            ctx->consume(n);
        }

        n->type = IdentifierTypeSpecifierNode::TryParse(ctx);
        if (n->type) {
            ctx->commit();
            n->extendLocation(n->type);
            return n;
        }

        n->type = ObjectTypeNode::TryParse(ctx);
        if (n->type) {
            ctx->commit();
            n->extendLocation(n->type);
            return n;
        }

        n->type = ArrayTypeNode::TryParse(ctx);
        if (n->type) {
            ctx->commit();
            n->extendLocation(n->type);
            return n;
        }

        n->type = FunctionTypeNode::TryParse(ctx);
        if (n->type) {
            ctx->commit();
            n->extendLocation(n->type);
            return n;
        }

        if (!n->type) {
            ctx->rollback();
            return n;
        }

        ctx->commit();

        if (isParenthesized) {
            if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseParen)) {
                ctx->logError("Expected ')'");
                n->m_isError = true;
            } else {
                ctx->consume(n);
            }
        }

        while (true) {
            TypeSpecifierNode* modified = TypeSpecifierNode::Create(ctx);
            modified->type = n;

            if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Asterisk)) {
                modified->mod = Modifier::Pointer;
                modified->extendLocation(ctx->get());
                ctx->consume();
            } else if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenBracket)) {
                modified->mod = Modifier::Array;
                modified->extendLocation(ctx->get());
                ctx->consume();

                if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseBracket)) {
                    ctx->logError("Expected ']'");
                    modified->m_isError = true;
                    return modified;
                }

                ctx->consume(n);
            } else {
                modified->destroy();
                break;
            }

            n = modified;
        }

        return n;
    }
};
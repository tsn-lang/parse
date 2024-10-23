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

        TypeSpecifierNode* n = nullptr;

        if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) {
            ctx->begin();

            FunctionTypeNode* ft = FunctionTypeNode::TryParse(ctx);
            if (ft && !ft->isError()) {
                ctx->commit();
                n = Create(ctx);
                n->type = ft;
                n->extendLocation(n->type);
            } else {
                ctx->rollback();
                if (ft) ft->destroy();

                const Token* openParenTok = ctx->get();
                ctx->consume(n);

                TypeSpecifierNode* n = TryParse(ctx);
                if (!n) ctx->rollback();
                n->extendLocation(openParenTok);

                if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseParen)) {
                    n->m_isError = true;
                    ctx->logError("Expected ')'");
                    return n;
                }

                ctx->consume(n);
                return TryParseModifiers(ctx, n);
            }
        }

        if (!n) {
            n = Create(ctx);
            n->type = IdentifierTypeSpecifierNode::TryParse(ctx);
            if (!n->type) n->type = ObjectTypeNode::TryParse(ctx);
            if (!n->type) n->type = ArrayTypeNode::TryParse(ctx);
            if (!n->type) n->type = FunctionTypeNode::TryParse(ctx);
        }
        
        if (n->type) {
            ctx->commit();
            n->extendLocation(n->type);
        } else {
            ctx->rollback();
            n->destroy();
            return nullptr;
        }

        return TryParseModifiers(ctx, n);
    }
    
    TypeSpecifierNode* TypeSpecifierNode::TryParseModifiers(Context* ctx, TypeSpecifierNode* base) {
        TypeSpecifierNode* n = base;

        while (true) {
            if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Asterisk)) {
                TypeSpecifierNode* modified = TypeSpecifierNode::Create(ctx);
                modified->extendLocation(n);
                modified->type = n;
                modified->mod = Modifier::Pointer;
                ctx->consume(modified);
                n = modified;
            } else if (ctx->match(TokenType::Symbol, TokenSubType::Operator_Exp)) {
                TypeSpecifierNode* modified = nullptr;
                modified = TypeSpecifierNode::Create(ctx);
                modified->extendLocation(n);
                modified->type = n;
                modified->mod = Modifier::Pointer;
                ctx->consume(modified);
                n = modified;
                
                modified = TypeSpecifierNode::Create(ctx);
                modified->extendLocation(n);
                modified->type = n;
                modified->mod = Modifier::Pointer;
                
                // Since this is technically the exponentiation operator...
                // the source location of the first pointer modifier must be
                // decremented
                n->m_location.endBufferPosition--;
                n->m_location.endColumn--;

                n = modified;
            } else if (ctx->matchAll({
                Match(TokenType::Symbol, TokenSubType::Symbol_OpenBracket),
                Match(TokenType::Symbol, TokenSubType::Symbol_CloseBracket)
            })) {
                TypeSpecifierNode* modified = TypeSpecifierNode::Create(ctx);
                modified->extendLocation(n);
                modified->type = n;
                modified->mod = Modifier::Array;
                ctx->consume(modified);
                ctx->consume(modified);
                n = modified;
            } else break;
        }

        return n;
    }
};
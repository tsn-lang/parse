#include <parse/ClassPropertyNode.h>
#include <parse/TypeSpecifierNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    ClassPropertyNode::ClassPropertyNode(Context* ctx) : Node(ctx), isPublic(true), isStatic(false), type(nullptr) {}
    void ClassPropertyNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ClassPropertyNode* ClassPropertyNode::Create(Context* ctx) { return new (ctx->allocNode()) ClassPropertyNode(ctx); }

    ClassPropertyNode* ClassPropertyNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        ctx->begin();

        ClassPropertyNode* n = Create(ctx);

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Private)) {
            n->extendLocation(ctx->get());
            ctx->consume();

            n->isPublic = false;
        } else if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Public)) {
            n->extendLocation(ctx->get());
            ctx->consume();

            n->isPublic = true;
        }

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Static)) {
            n->extendLocation(ctx->get());
            ctx->consume();

            n->isStatic = true;
        }

        if (!ctx->match(TokenType::Identifier)) {
            ctx->rollback();
            n->destroy();
            return nullptr;
        }
        
        const Token* nameTok = ctx->get();
        ctx->consume();
        n->name = nameTok->toString();
        n->extendLocation(nameTok);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Colon)) {
            ctx->rollback();
            n->destroy();
            return nullptr;
        }

        // can be sure now, no other rule starts with the pattern:
        //     ['private' | 'public'], ['static'], identifier, ':'
        ctx->commit();

        n->extendLocation(ctx->get());
        ctx->consume();

        n->type = TypeSpecifierNode::TryParse(ctx);
        if (!n->type) {
            ctx->logError("Expected type specifier after ':'");
            n->m_isError = true;
            return n;
        }

        if (n->type->isError()) {
            n->type->destroy();
            n->type = nullptr;
            ctx->skipTo(TokenType::EndOfStatement);
        } else {
            n->extendLocation(n->type);
        }

        if (!ctx->match(TokenType::EndOfStatement)) {
            ctx->logError("Expected ';'");
            n->m_isError = true;
            return n;
        }

        ctx->consume();

        return n;
    }
};
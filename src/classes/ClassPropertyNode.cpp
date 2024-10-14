#include <parse/classes/ClassPropertyNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    ClassPropertyNode::ClassPropertyNode(Context* ctx) : Node(ctx), isPublic(true), isStatic(false), type(nullptr) {}
    ClassPropertyNode::~ClassPropertyNode() {}
    void ClassPropertyNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ClassPropertyNode* ClassPropertyNode::Create(Context* ctx) { return new (ctx->allocNode()) ClassPropertyNode(ctx); }

    ClassPropertyNode* ClassPropertyNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        ctx->begin();

        ClassPropertyNode* n = Create(ctx);

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Private)) {
            ctx->consume(n);

            n->isPublic = false;
        } else if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Public)) {
            ctx->consume(n);

            n->isPublic = true;
        }

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Static)) {
            ctx->consume(n);

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

        ctx->consume(n);

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
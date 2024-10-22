#include <parse/declarations/EnumNode.h>
#include <parse/Context.h>
#include <parse/declarations/EnumFieldNode.h>
#include <tokenize/Token.h>
#include <utils/Array.hpp>

namespace parse {
    EnumNode::EnumNode(Context* ctx) : Node(ctx, NodeType::EnumNode), isDeclaration(false) {}
    EnumNode::~EnumNode() {}
    void EnumNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    EnumNode* EnumNode::Create(Context* ctx) { return new (ctx->allocNode()) EnumNode(ctx); }

    EnumNode* EnumNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Enum)) return nullptr;

        EnumNode* n = Create(ctx);
        ctx->consume(n);

        if (!ctx->match(TokenType::Identifier)) {
            ctx->logError("Expected identifier after 'enum'");
            n->m_isError = true;
            return n;
        }
        
        const Token* nameTok = ctx->get();
        n->name = nameTok->toString();
        n->extendLocation(nameTok);
        ctx->consume();

        if (ctx->match(TokenType::EndOfStatement)) {
            ctx->consume(n);
            n->isDeclaration = true;
            return n;
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenBrace)) {
            ctx->logError("Expected '{'");
            n->m_isError = true;
            return n;
        }

        ctx->consume(n);

        EnumFieldNode* field = EnumFieldNode::TryParse(ctx);
        while (field) {
            if (field->isError()) {
                field->destroy();
                ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseBrace);
                break;
            }

            n->fields.push(field);
            n->extendLocation(field);

            if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Comma)) {
                ctx->consume(n);
                field = EnumFieldNode::TryParse(ctx);

                if (!field) {
                    ctx->logError("Expected typed parameter after ','");
                    n->m_isError = true;
                    ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseBrace);
                    break;
                }
            } else field = nullptr;
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)) {
            ctx->logError("Expected '}'");
            n->m_isError = true;
            return n;
        }

        ctx->consume(n);

        return n;
    }
};
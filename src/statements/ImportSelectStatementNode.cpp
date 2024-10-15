#include <parse/statements/ImportSelectStatementNode.h>
#include <parse/misc/IdentifierNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    ImportSelectStatementNode::ImportSelectStatementNode(Context* ctx) : Node(ctx, NodeType::ImportSelectStatementNode) {}
    void ImportSelectStatementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ImportSelectStatementNode* ImportSelectStatementNode::Create(Context* ctx) { return new (ctx->allocNode()) ImportSelectStatementNode(ctx); }

    ImportSelectStatementNode* ImportSelectStatementNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Import)) return nullptr;

        ctx->begin();

        ImportSelectStatementNode* n = Create(ctx);
        ctx->consume(n);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenBrace)) {
            if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Asterisk)) {
                ctx->logError("Expected '*' or import list");
                n->m_isError = true;

                ctx->rollback();
                return n;
            }

            ctx->rollback();
            n->destroy();
            return nullptr;
        }

        ctx->commit();
        ctx->consume(n);

        IdentifierNode* identifier = IdentifierNode::TryParse(ctx);
        while (identifier) {
            if (!n) n = Create(ctx);

            if (identifier->isError()) {
                n->m_isError = true;
                identifier->destroy();
                ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseBrace);
                break;
            }

            n->importList.push(identifier);
            n->extendLocation(identifier);

            if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Comma)) {
                ctx->consume(n);
                identifier = IdentifierNode::TryParse(ctx);

                if (!identifier) {
                    ctx->logError("Expected identifier after ','");
                    n->m_isError = true;
                    ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseBrace);
                    break;
                }
            } else identifier = nullptr;
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)) {
            ctx->logError("Expected '}'");
            n->m_isError = true;

            if (!ctx->skipToAnyMatched({
                Match(TokenType::Keyword, TokenSubType::Keyword_From),
                Match(TokenType::EndOfStatement)
            })) {
                return n;
            }
        } else ctx->consume(n);

        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_From)) {
            ctx->logError("Expected 'from'");
            n->m_isError = true;

            ctx->skipTo(TokenType::EndOfStatement);
            return n;
        }

        ctx->consume(n);

        if (!ctx->match(TokenType::Literal, TokenSubType::Literal_String)) {
            ctx->logError("Expected string");
            n->m_isError = true;
            return n;
        }

        n->moduleId = ctx->get()->getContentString();
        ctx->consume(n);

        return n;
    }
};
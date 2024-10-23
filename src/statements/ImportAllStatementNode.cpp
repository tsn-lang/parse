#include <parse/statements/ImportAllStatementNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    ImportAllStatementNode::ImportAllStatementNode(Context* ctx) : Node(ctx, NodeType::ImportAllStatementNode) {}
    ImportAllStatementNode::~ImportAllStatementNode() {}
    void ImportAllStatementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ImportAllStatementNode* ImportAllStatementNode::Create(Context* ctx) { return new (ctx->allocNode()) ImportAllStatementNode(ctx); }

    ImportAllStatementNode* ImportAllStatementNode::TryParse(Context* ctx) {
        if (!ctx->matchAll({
            Match(TokenType::Keyword, TokenSubType::Keyword_Import),
            Match(TokenType::Symbol, TokenSubType::Symbol_Asterisk)
        })) return nullptr;

        ImportAllStatementNode* n = Create(ctx);
        
        ctx->consume(n); // import
        ctx->consume(n); // *

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_As)) {
            ctx->consume(n);

            if (ctx->match(TokenType::Identifier)) {
                n->alias = ctx->get()->toString();
                ctx->consume(n);
            } else {
                ctx->logError("Expected identifier");
                n->m_isError = true;
                if (!ctx->skipToAnyMatched({
                    Match(TokenType::Keyword, TokenSubType::Keyword_From),
                    Match(TokenType::EndOfStatement)
                })) {
                    return n;
                }
            }
        }

        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_From)) {
            ctx->logError("Expected 'from'");
            n->m_isError = true;
            if (ctx->skipTo(TokenType::EndOfStatement)) {
                ctx->consume(n);
            }
            return n;
        }
        
        ctx->consume(n);

        if (!ctx->match(TokenType::Literal, TokenSubType::Literal_String)) {
            ctx->logError("Expected string");
            n->m_isError = true;

            if (ctx->skipTo(TokenType::EndOfStatement)) {
                ctx->consume(n);
            }
            return n;
        }
        
        String id = ctx->get()->toString();
        n->moduleId = String::View(id.c_str() + 1, id.size() - 2);
        ctx->consume(n);

        return n;
    }
};
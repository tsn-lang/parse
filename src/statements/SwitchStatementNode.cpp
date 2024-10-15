#include <parse/statements/SwitchStatementNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/statements/SwitchCaseNode.h>
#include <parse/Context.h>
#include <utils/Array.hpp>

namespace parse {
    SwitchStatementNode::SwitchStatementNode(Context* ctx) : Node(ctx, NodeType::SwitchStatementNode), value(nullptr) {}
    SwitchStatementNode::~SwitchStatementNode() {}
    void SwitchStatementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    SwitchStatementNode* SwitchStatementNode::Create(Context* ctx) { return new (ctx->allocNode()) SwitchStatementNode(ctx); }

    SwitchStatementNode* SwitchStatementNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Switch)) return nullptr;

        SwitchStatementNode* n = Create(ctx);
        ctx->consume(n);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) {
            ctx->logError("Expected '('");
            n->m_isError = true;
        }

        n->value = ExpressionNode::TryParse(ctx);
        if (!n->value) {
            ctx->logError("Expected expression");
            n->m_isError = true;

            if (!ctx->skipToAnyMatched({
                Match(TokenType::Symbol, TokenSubType::Symbol_CloseParen),
                Match(TokenType::Symbol, TokenSubType::Symbol_OpenBrace)
            })) {
                return n;
            }
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseParen)) {
            ctx->logError("Expected ')'");
            n->m_isError = true;
        } else ctx->consume(n);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenBrace)) {
            ctx->logError("Expected '{'");
            n->m_isError = true;
        } else ctx->consume(n);

        SwitchCaseNode* caseBlock = SwitchCaseNode::TryParse(ctx);
        while (caseBlock) {
            if (caseBlock->isError()) {
                n->m_isError = true;
                n->extendLocation(caseBlock);
                caseBlock->destroy();

                if (!ctx->skipToAnyMatched({
                    Match(TokenType::Keyword, TokenSubType::Keyword_Case),
                    Match(TokenType::Keyword, TokenSubType::Keyword_Default),
                    Match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)
                })) {
                    return n;
                }

                break;
            } else {
                n->cases.push(caseBlock);
                n->extendLocation(caseBlock);
            }

            caseBlock = SwitchCaseNode::TryParse(ctx);
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)) {
            ctx->logError("Expected '}'");
            n->m_isError = true;
        }

        return n;
    }
};
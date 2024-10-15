#include <parse/statements/SwitchCaseNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/proxies/StatementNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    SwitchCaseNode::SwitchCaseNode(Context* ctx) : Node(ctx, NodeType::SwitchCaseNode), value(nullptr), body(nullptr) {}
    void SwitchCaseNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    SwitchCaseNode* SwitchCaseNode::Create(Context* ctx) { return new (ctx->allocNode()) SwitchCaseNode(ctx); }

    SwitchCaseNode* SwitchCaseNode::TryParse(Context* ctx) {
        if (!ctx->matchAny({
            Match(TokenType::Keyword, TokenSubType::Keyword_Case),
            Match(TokenType::Keyword, TokenSubType::Keyword_Default)
        })) return nullptr;

        SwitchCaseNode* n = Create(ctx);
        if (ctx->get()->subType == i32(TokenSubType::Keyword_Case)) {
            ctx->consume(n);
            n->value = ExpressionNode::TryParse(ctx);
            if (!n->value) {
                n->m_isError = true;
                ctx->logError("Expected expression");
                if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_Colon)) {
                    return n;
                }
            } else n->m_isError = n->value->isError();
        } else {
            ctx->consume(n);
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Colon)) {
            n->m_isError = true;
            ctx->logError("Expected ':'");
            return n;
        }

        ctx->consume(n);

        if (ctx->matchAny({
            Match(TokenType::Keyword, TokenSubType::Keyword_Case),
            Match(TokenType::Keyword, TokenSubType::Keyword_Default),
            Match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)
        })) {
            // Valid next tokens if this case has no body, this will save some time
            // rather than trying to parse a statement
            return n;
        }

        n->body = StatementNode::TryParse(ctx);
        n->m_isError = n->body->isError();

        return n;
    }
};
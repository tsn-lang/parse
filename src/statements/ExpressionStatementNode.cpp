#include <parse/statements/ExpressionStatementNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/Context.h>

namespace parse {
    ExpressionStatementNode::ExpressionStatementNode(Context* ctx) : Node(ctx) {}
    ExpressionStatementNode::~ExpressionStatementNode() {}
    void ExpressionStatementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ExpressionStatementNode* ExpressionStatementNode::Create(Context* ctx) { return new (ctx->allocNode()) ExpressionStatementNode(ctx); }

    ExpressionStatementNode* ExpressionStatementNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        ExpressionStatementNode* n = nullptr;

        ExpressionNode* expr = ExpressionNode::TryParse(ctx);
        while (expr) {
            if (!n) n = Create(ctx);

            if (expr->isError()) {
                n->m_isError = true;
                expr->destroy();
                ctx->skipTo(TokenType::EndOfStatement);
                break;
            }

            n->expressions.push(expr);
            n->extendLocation(expr);

            if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Comma)) {
                ctx->consume(n);
                expr = ExpressionNode::TryParse(ctx);

                if (!expr) {
                    ctx->logError("Expected expression after ','");
                    n->m_isError = true;
                    ctx->skipTo(TokenType::EndOfStatement);
                    break;
                }
            } else expr = nullptr;
        }

        return n;
    }
};
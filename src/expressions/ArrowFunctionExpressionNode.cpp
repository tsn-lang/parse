#include <parse/expressions/ArrowFunctionExpressionNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/misc/ParameterListNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/Context.h>

namespace parse {
    ArrowFunctionExpressionNode::ArrowFunctionExpressionNode(Context* ctx)
        : Node(ctx), parameters(nullptr), body(nullptr), returnType(nullptr)
    {}
    void ArrowFunctionExpressionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ArrowFunctionExpressionNode* ArrowFunctionExpressionNode::Create(Context* ctx) { return new (ctx->allocNode()) ArrowFunctionExpressionNode(ctx); }

    ArrowFunctionExpressionNode* ArrowFunctionExpressionNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) return nullptr;

        ArrowFunctionExpressionNode* n = Create(ctx);

        n->parameters = ParameterListNode::TryParse(ctx);
        if (!n->parameters) {
            n->m_isError = true;
            ctx->logError("Expected parameter list");
            if (!ctx->skipToAnyMatched({
                Match(TokenType::Symbol, TokenSubType::Symbol_Arrow),
                Match(TokenType::Symbol, TokenSubType::Symbol_Colon)
            })) return n;
        } else {
            n->m_isError = n->parameters->isError();
            n->extendLocation(n->parameters);
        }

        if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Colon)) {
            ctx->consume(n);

            n->returnType = TypeSpecifierNode::TryParse(ctx);
            if (!n->returnType) {
                ctx->logError("Expected return type specifier");
                return n;
            }
        }

        if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenBrace)) {
            n->body = StatementBlockNode::TryParse(ctx);
            if (!n->body) {
                ctx->logError("Expected function body");
                return n;
            }
        } else {
            n->body = ExpressionNode::TryParse(ctx);
            if (!n->body) {
                n->m_isError = true;
                ctx->logError("Expected function body");
                return n;
            }
        }

        n->m_isError = n->body->isError();

        return n;
    }
};
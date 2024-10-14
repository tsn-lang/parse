#include <parse/expressions/FunctionExpressionNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/misc/ParameterListNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <parse/Context.h>

namespace parse {
    FunctionExpressionNode::FunctionExpressionNode(Context* ctx) : Node(ctx), parameters(nullptr), returnType(nullptr), body(nullptr) {}
    void FunctionExpressionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    FunctionExpressionNode* FunctionExpressionNode::Create(Context* ctx) { return new (ctx->allocNode()) FunctionExpressionNode(ctx); }

    FunctionExpressionNode* FunctionExpressionNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Function)) return nullptr;

        FunctionExpressionNode* n = Create(ctx);
        ctx->consume(n);

        n->parameters = ParameterListNode::TryParse(ctx);
        if (!n->parameters) {
            n->m_isError = true;
            ctx->logError("Expected parameter list");
            if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_OpenBrace)) return n;
        } else {
            n->m_isError = n->parameters->isError();
            n->extendLocation(n->parameters);
        }

        if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Colon)) {
            ctx->consume(n);

            n->returnType = TypeSpecifierNode::TryParse(ctx);
            if (!n->returnType) {
                ctx->logError("Expected return type specifier");
                if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_OpenBrace)) return n;
            }
        }

        n->body = StatementBlockNode::TryParse(ctx);
        if (!n->body) {
            n->m_isError = true;
            ctx->logError("Expected function body");
            if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)) return n;
            
            ctx->consume(n);
            return n;
        }

        n->m_isError = n->body->isError();
        return n;
    }
};
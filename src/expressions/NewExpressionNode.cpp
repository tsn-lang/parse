#include <parse/expressions/NewExpressionNode.h>
#include <parse/expressions/MemberExpressionNode.h>
#include <parse/expressions/ExpressionSequenceNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/Context.h>

namespace parse {
    NewExpressionNode::NewExpressionNode(Context* ctx) : Node(ctx), type(nullptr), arguments(nullptr), destination(nullptr) {}
    void NewExpressionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    NewExpressionNode* NewExpressionNode::Create(Context* ctx) { return new (ctx->allocNode()) NewExpressionNode(ctx); }

    Node* NewExpressionNode::TryParse(Context* ctx) {
        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_New)) {
            return MemberExpressionNode::TryParse(ctx);
        }

        NewExpressionNode* n = Create(ctx);
        ctx->consume(n);

        n->type = TypeSpecifierNode::TryParse(ctx);
        if (!n->type) {
            n->m_isError = true;
            ctx->logError("Expected type specifier");
            
            if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) return n;
        }
        n->m_isError = n->m_isError || n->type->isError();
        n->extendLocation(n->type);

        n->arguments = ExpressionSequenceNode::TryParseParenthesized(ctx);
        if (!n->arguments) {
            n->m_isError = true;
            ctx->logError("Expected argument list");
            return n;
        }
        n->m_isError = n->m_isError || n->arguments->isError();
        n->extendLocation(n->arguments);

        if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Arrow)) {
            ctx->consume(n);
            n->destination = ExpressionNode::TryParse(ctx);
            if (!n->destination) {
                n->m_isError = true;
                ctx->logError("Expected expression");
                return n;
            }

            n->extendLocation(n->destination);
            n->m_isError = n->m_isError || n->destination->isError();
        }

        return n;
    }
};
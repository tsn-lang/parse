#include <parse/expressions/TypeInfoExpressionNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/Context.h>

namespace parse {
    TypeInfoExpressionNode::TypeInfoExpressionNode(Context* ctx) : Node(ctx, NodeType::TypeInfoExpressionNode), type(nullptr) {}
    void TypeInfoExpressionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    TypeInfoExpressionNode* TypeInfoExpressionNode::Create(Context* ctx) { return new (ctx->allocNode()) TypeInfoExpressionNode(ctx); }

    TypeInfoExpressionNode* TypeInfoExpressionNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_TypeInfo)) return nullptr;

        TypeInfoExpressionNode* n = Create(ctx);
        ctx->consume(n);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Operator_LessThan)) {
            n->m_isError = true;
            ctx->logError("Expected '<'");
            return n;
        }

        ctx->consume(n);

        n->type = TypeSpecifierNode::TryParse(ctx);
        if (!n->type) {
            n->m_isError = true;
            ctx->logError("Expected type specifier");

            if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Operator_GreaterThan)) {
                return n;
            }
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Operator_GreaterThan)) {
            n->m_isError = true;
            ctx->logError("Expected '>'");
            return n;
        } 

        return n;
    }
};
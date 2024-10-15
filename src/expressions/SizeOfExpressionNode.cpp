#include <parse/expressions/SizeOfExpressionNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/Context.h>

namespace parse {
    SizeOfExpressionNode::SizeOfExpressionNode(Context* ctx) : Node(ctx, NodeType::SizeOfExpressionNode), type(nullptr) {}
    void SizeOfExpressionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    SizeOfExpressionNode* SizeOfExpressionNode::Create(Context* ctx) { return new (ctx->allocNode()) SizeOfExpressionNode(ctx); }

    SizeOfExpressionNode* SizeOfExpressionNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_SizeOf)) return nullptr;

        SizeOfExpressionNode* n = Create(ctx);
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
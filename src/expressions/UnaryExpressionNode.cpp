#include <parse/expressions/UnaryExpressionNode.h>
#include <parse/expressions/CallExpressionNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    UnaryExpressionNode::UnaryExpressionNode(Context* ctx) : Node(ctx), expr(nullptr), operation(OperatorType::Invalid) {}
    void UnaryExpressionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    UnaryExpressionNode* UnaryExpressionNode::Create(Context* ctx) { return new (ctx->allocNode()) UnaryExpressionNode(ctx); }

    Node* UnaryExpressionNode::TryParsePostfixUnaryExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* call = CallExpressionNode::TryParse(ctx);
        if (!call) return nullptr;

        if (!ctx->matchAny({
            Match(TokenType::Symbol, TokenSubType::Operator_Increment),
            Match(TokenType::Symbol, TokenSubType::Operator_Decrement)
        })) return call;

        UnaryExpressionNode* n = Create(ctx);
        if (TokenSubType(ctx->get()->subType) == TokenSubType::Operator_Increment) {
            n->operation = OperatorType::PostIncrement;
        } else {
            n->operation = OperatorType::PostDecrement;
        }

        ctx->consume(n);

        n->expr = call;
        n->extendLocation(n->expr);
        n->m_isError = n->expr->isError();

        return n;
    }

    Node* UnaryExpressionNode::TryParsePrefixUnaryExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        if (!ctx->matchAny({
            Match(TokenType::Symbol, TokenSubType::Operator_Increment),
            Match(TokenType::Symbol, TokenSubType::Operator_Decrement),
            Match(TokenType::Symbol, TokenSubType::Operator_BitwiseInvert),
            Match(TokenType::Symbol, TokenSubType::Operator_Not),
            Match(TokenType::Symbol, TokenSubType::Operator_Sub),
            Match(TokenType::Symbol, TokenSubType::Operator_Mul)
        })) return TryParsePostfixUnaryExpression(ctx);

        UnaryExpressionNode* n = Create(ctx);
        switch (TokenSubType(ctx->get()->subType)) {
            case TokenSubType::Operator_Increment: {
                n->operation = OperatorType::PreIncrement;
                break;
            }
            case TokenSubType::Operator_Decrement: {
                n->operation = OperatorType::PreDecrement;
                break;
            }
            case TokenSubType::Operator_Sub: {
                n->operation = OperatorType::Negate;
                break;
            }
            case TokenSubType::Symbol_Asterisk: {
                n->operation = OperatorType::Dereference;
                break;
            }
            default: {
                n->operation = OperatorType(ctx->get()->subType);
                break;
            }
        }

        ctx->consume(n);

        n->expr = TryParsePostfixUnaryExpression(ctx);
        if (!n->expr) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            return n;
        }

        n->extendLocation(n->expr);
        n->m_isError = n->expr->isError();

        return n;
    }
};
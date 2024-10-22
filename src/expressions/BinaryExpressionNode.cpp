#include <parse/expressions/BinaryExpressionNode.h>
#include <parse/expressions/ConditionalExpressionNode.h>
#include <parse/expressions/CastExpressionNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    BinaryExpressionNode::BinaryExpressionNode(Context* ctx)
        : Node(ctx, NodeType::BinaryExpressionNode), lhs(nullptr), rhs(nullptr), operation(OperatorType::Invalid)
    {}
    void BinaryExpressionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    BinaryExpressionNode* BinaryExpressionNode::Create(Context* ctx) { return new (ctx->allocNode()) BinaryExpressionNode(ctx); }

    Node* BinaryExpressionNode::TryParseExponentExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* expr = CastExpressionNode::TryParse(ctx);
        if (!expr) return nullptr;

        if (!ctx->match(TokenType::Symbol, TokenSubType::Operator_Exp)) return expr;

        BinaryExpressionNode* n = Create(ctx);
        n->operation = OperatorType::Exp;
        n->lhs = expr;
        n->m_isError = n->lhs->isError();
        n->extendLocation(n->lhs);
        ctx->consume(n);

        n->rhs = ExpressionNode::TryParse(ctx);
        if (!n->rhs) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            return n;
        }
        n->m_isError = n->m_isError || n->rhs->isError();

        n->extendLocation(n->rhs);
        return n;
    }

    Node* BinaryExpressionNode::TryParseMultiplicativeExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* expr = TryParseExponentExpression(ctx);
        if (!expr) return nullptr;

        if (!ctx->matchAny({
            Match(TokenType::Symbol, TokenSubType::Operator_Mul),
            Match(TokenType::Symbol, TokenSubType::Operator_Div),
            Match(TokenType::Symbol, TokenSubType::Operator_Mod)
        })) return expr;

        BinaryExpressionNode* n = Create(ctx);
        n->operation = OperatorType(ctx->get()->subType);
        n->lhs = expr;
        n->m_isError = n->lhs->isError();
        n->extendLocation(n->lhs);
        ctx->consume(n);

        n->rhs = ExpressionNode::TryParse(ctx);
        if (!n->rhs) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            return n;
        }
        n->m_isError = n->m_isError || n->rhs->isError();

        n->extendLocation(n->rhs);
        return n;
    }

    Node* BinaryExpressionNode::TryParseAdditiveExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* expr = TryParseMultiplicativeExpression(ctx);
        if (!expr) return nullptr;

        if (!ctx->matchAny({
            Match(TokenType::Symbol, TokenSubType::Operator_Add),
            Match(TokenType::Symbol, TokenSubType::Operator_Sub)
        })) return expr;

        BinaryExpressionNode* n = Create(ctx);
        n->operation = OperatorType(ctx->get()->subType);
        n->lhs = expr;
        n->m_isError = n->lhs->isError();
        n->extendLocation(n->lhs);
        ctx->consume(n);

        n->rhs = ExpressionNode::TryParse(ctx);
        if (!n->rhs) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            return n;
        }
        n->m_isError = n->m_isError || n->rhs->isError();

        n->extendLocation(n->rhs);
        return n;
    }

    Node* BinaryExpressionNode::TryParseBitShiftExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* expr = TryParseAdditiveExpression(ctx);
        if (!expr) return nullptr;

        if (!ctx->matchAny({
            Match(TokenType::Symbol, TokenSubType::Operator_ShiftLeft),
            Match(TokenType::Symbol, TokenSubType::Operator_ShiftRight)
        })) return expr;

        BinaryExpressionNode* n = Create(ctx);
        n->operation = OperatorType(ctx->get()->subType);
        n->lhs = expr;
        n->m_isError = n->lhs->isError();
        n->extendLocation(n->lhs);
        ctx->consume(n);

        n->rhs = ExpressionNode::TryParse(ctx);
        if (!n->rhs) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            return n;
        }
        n->m_isError = n->m_isError || n->rhs->isError();

        n->extendLocation(n->rhs);
        return n;
    }

    Node* BinaryExpressionNode::TryParseRelationalExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* expr = TryParseBitShiftExpression(ctx);
        if (!expr) return nullptr;

        if (!ctx->matchAny({
            Match(TokenType::Symbol, TokenSubType::Operator_LessThan),
            Match(TokenType::Symbol, TokenSubType::Operator_GreaterThan),
            Match(TokenType::Symbol, TokenSubType::Operator_LessThanEq),
            Match(TokenType::Symbol, TokenSubType::Operator_GreaterThanEq)
        })) return expr;

        BinaryExpressionNode* n = Create(ctx);
        n->operation = OperatorType(ctx->get()->subType);
        n->lhs = expr;
        n->m_isError = n->lhs->isError();
        n->extendLocation(n->lhs);
        ctx->consume(n);

        n->rhs = ExpressionNode::TryParse(ctx);
        if (!n->rhs) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            return n;
        }
        n->m_isError = n->m_isError || n->rhs->isError();

        n->extendLocation(n->rhs);
        return n;
    }

    Node* BinaryExpressionNode::TryParseEqualityExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* expr = TryParseRelationalExpression(ctx);
        if (!expr) return nullptr;

        if (!ctx->matchAny({
            Match(TokenType::Symbol, TokenSubType::Operator_Equality),
            Match(TokenType::Symbol, TokenSubType::Operator_Inequality)
        })) return expr;

        BinaryExpressionNode* n = Create(ctx);
        n->operation = OperatorType(ctx->get()->subType);
        n->lhs = expr;
        n->m_isError = n->lhs->isError();
        n->extendLocation(n->lhs);
        ctx->consume(n);

        n->rhs = ExpressionNode::TryParse(ctx);
        if (!n->rhs) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            return n;
        }
        n->m_isError = n->m_isError || n->rhs->isError();

        n->extendLocation(n->rhs);
        return n;
    }

    Node* BinaryExpressionNode::TryParseBitwiseAndExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* expr = TryParseEqualityExpression(ctx);
        if (!expr) return nullptr;

        if (!ctx->match(TokenType::Symbol, TokenSubType::Operator_BitwiseAnd)) return expr;

        BinaryExpressionNode* n = Create(ctx);
        n->operation = OperatorType::BitwiseAnd;
        n->lhs = expr;
        n->m_isError = n->lhs->isError();
        n->extendLocation(n->lhs);
        ctx->consume(n);

        n->rhs = ExpressionNode::TryParse(ctx);
        if (!n->rhs) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            return n;
        }
        n->m_isError = n->m_isError || n->rhs->isError();

        n->extendLocation(n->rhs);
        return n;
    }

    Node* BinaryExpressionNode::TryParseBitwiseXorExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* expr = TryParseBitwiseAndExpression(ctx);
        if (!expr) return nullptr;

        if (!ctx->match(TokenType::Symbol, TokenSubType::Operator_BitwiseXOr)) return expr;

        BinaryExpressionNode* n = Create(ctx);
        n->operation = OperatorType::BitwiseXOr;
        n->lhs = expr;
        n->m_isError = n->lhs->isError();
        n->extendLocation(n->lhs);
        ctx->consume(n);

        n->rhs = ExpressionNode::TryParse(ctx);
        if (!n->rhs) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            return n;
        }
        n->m_isError = n->m_isError || n->rhs->isError();

        n->extendLocation(n->rhs);
        return n;
    }

    Node* BinaryExpressionNode::TryParseBitwiseOrExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* expr = TryParseBitwiseXorExpression(ctx);
        if (!expr) return nullptr;

        if (!ctx->match(TokenType::Symbol, TokenSubType::Operator_BitwiseOr)) return expr;

        BinaryExpressionNode* n = Create(ctx);
        n->operation = OperatorType::BitwiseOr;
        n->lhs = expr;
        n->m_isError = n->lhs->isError();
        n->extendLocation(n->lhs);
        ctx->consume(n);

        n->rhs = ExpressionNode::TryParse(ctx);
        if (!n->rhs) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            return n;
        }
        n->m_isError = n->m_isError || n->rhs->isError();

        n->extendLocation(n->rhs);
        return n;
    }

    Node* BinaryExpressionNode::TryParseLogicalAndExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* expr = TryParseBitwiseOrExpression(ctx);
        if (!expr) return nullptr;

        if (!ctx->match(TokenType::Symbol, TokenSubType::Operator_LogicalAnd)) return expr;

        BinaryExpressionNode* n = Create(ctx);
        n->operation = OperatorType::LogicalAnd;
        n->lhs = expr;
        n->m_isError = n->lhs->isError();
        n->extendLocation(n->lhs);
        ctx->consume(n);

        n->rhs = ExpressionNode::TryParse(ctx);
        if (!n->rhs) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            return n;
        }
        n->m_isError = n->m_isError || n->rhs->isError();

        n->extendLocation(n->rhs);
        return n;
    }

    Node* BinaryExpressionNode::TryParseLogicalOrExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* expr = TryParseLogicalAndExpression(ctx);
        if (!expr) return nullptr;

        if (!ctx->match(TokenType::Symbol, TokenSubType::Operator_LogicalOr)) return expr;

        BinaryExpressionNode* n = Create(ctx);
        n->operation = OperatorType::LogicalOr;
        n->lhs = expr;
        n->m_isError = n->lhs->isError();
        n->extendLocation(n->lhs);
        ctx->consume(n);

        n->rhs = ExpressionNode::TryParse(ctx);
        if (!n->rhs) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            return n;
        }
        n->m_isError = n->m_isError || n->rhs->isError();

        n->extendLocation(n->rhs);
        return n;
    }

    Node* BinaryExpressionNode::TryParseAssignmentExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* cond = ConditionalExpressionNode::TryParse(ctx);
        if (!cond) return nullptr;

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Equal)) return cond;

        BinaryExpressionNode* n = Create(ctx);
        n->operation = OperatorType::Assign;
        n->lhs = cond;
        n->m_isError = n->lhs->isError();
        n->extendLocation(n->lhs);
        ctx->consume(n);

        n->rhs = ExpressionNode::TryParse(ctx);
        if (!n->rhs) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            return n;
        }
        n->m_isError = n->m_isError || n->rhs->isError();

        n->extendLocation(n->rhs);
        return n;
    }

    Node* BinaryExpressionNode::TryParseAssignmentOperatorExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* assignment = TryParseAssignmentExpression(ctx);
        if (!assignment) return nullptr;

        if (!ctx->match(TokenType::Symbol)) return assignment;

        const Token* op = ctx->get();
        switch (TokenSubType(op->subType)) {
            case TokenSubType::Operator_AddEq: break;
            case TokenSubType::Operator_SubEq: break;
            case TokenSubType::Operator_MulEq: break;
            case TokenSubType::Operator_DivEq: break;
            case TokenSubType::Operator_ModEq: break;
            case TokenSubType::Operator_ExpEq: break;
            case TokenSubType::Operator_BitwiseAndEq: break;
            case TokenSubType::Operator_BitwiseOrEq: break;
            case TokenSubType::Operator_BitwiseXOrEq: break;
            case TokenSubType::Operator_ShiftLeftEq: break;
            case TokenSubType::Operator_ShiftRightEq: break;
            case TokenSubType::Operator_LogicalAndEq: break;
            case TokenSubType::Operator_LogicalOrEq: break;
            default: return assignment;
        }

        BinaryExpressionNode* n = Create(ctx);
        n->operation = OperatorType(op->subType);
        n->lhs = assignment;
        n->m_isError = n->lhs->isError();
        n->extendLocation(n->lhs);
        ctx->consume(n);

        n->rhs = ExpressionNode::TryParse(ctx);
        if (!n->rhs) {
            n->m_isError = true;
            ctx->logError("Expected expression");
            return n;
        }
        n->m_isError = n->m_isError || n->rhs->isError();

        n->extendLocation(n->rhs);
        return n;
    }
};
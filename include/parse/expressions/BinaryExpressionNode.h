#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class BinaryExpressionNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static BinaryExpressionNode* Create(Context* ctx);
            static Node* TryParseExponentExpression(Context* ctx);
            static Node* TryParseMultiplicativeExpression(Context* ctx);
            static Node* TryParseAdditiveExpression(Context* ctx);
            static Node* TryParseBitShiftExpression(Context* ctx);
            static Node* TryParseRelationalExpression(Context* ctx);
            static Node* TryParseEqualityExpression(Context* ctx);
            static Node* TryParseBitwiseAndExpression(Context* ctx);
            static Node* TryParseBitwiseXorExpression(Context* ctx);
            static Node* TryParseBitwiseOrExpression(Context* ctx);
            static Node* TryParseLogicalAndExpression(Context* ctx);
            static Node* TryParseLogicalOrExpression(Context* ctx);
            static Node* TryParseAssignmentExpression(Context* ctx);
            static Node* TryParseAssignmentOperatorExpression(Context* ctx);

            Node* lhs;
            Node* rhs;
            OperatorType operation;

        private:
            BinaryExpressionNode(Context* ctx);
    };
};
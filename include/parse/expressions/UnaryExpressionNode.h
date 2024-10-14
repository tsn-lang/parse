#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class UnaryExpressionNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static UnaryExpressionNode* Create(Context* ctx);
            static Node* TryParsePostfixUnaryExpression(Context* ctx);
            static Node* TryParsePrefixUnaryExpression(Context* ctx);

            Node* expr;
            OperatorType operation;

        private:
            UnaryExpressionNode(Context* ctx);
    };
};
#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class ExpressionNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ExpressionNode* Create(Context* ctx);
            static ExpressionNode* TryParse(Context* ctx);
            static ExpressionNode* TryParseParenthesized(Context* ctx);
            static Node* TryParsePrimaryExpression(Context* ctx);

            Node* expr;
        private:
            ExpressionNode(Context* ctx);
    };
};
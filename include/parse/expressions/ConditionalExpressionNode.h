#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class ConditionalExpressionNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ConditionalExpressionNode* Create(Context* ctx);
            static Node* TryParse(Context* ctx);

            Node* condition;
            Node* valueOnTrue;
            Node* valueOnFalse;

        private:
            ConditionalExpressionNode(Context* ctx);
    };
};
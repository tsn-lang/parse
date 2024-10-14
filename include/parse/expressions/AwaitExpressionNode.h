#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class AwaitExpressionNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static AwaitExpressionNode* Create(Context* ctx);
            static Node* TryParse(Context* ctx);

            Node* expr;

        private:
            AwaitExpressionNode(Context* ctx);
    };
};
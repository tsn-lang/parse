#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class CallExpressionNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static CallExpressionNode* Create(Context* ctx);
            static Node* TryParse(Context* ctx);
            static CallExpressionNode* TryParseTail(Context* ctx, Node* base);

            Node* callee;
            ExpressionSequenceNode* arguments;

        private:
            CallExpressionNode(Context* ctx);
    };
};
#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class ArrowFunctionExpressionNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ArrowFunctionExpressionNode* Create(Context* ctx);
            static ArrowFunctionExpressionNode* TryParse(Context* ctx);

            ParameterListNode* parameters;
            TypeSpecifierNode* returnType;
            Node* body;

        private:
            ArrowFunctionExpressionNode(Context* ctx);
    };
};
#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class FunctionExpressionNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static FunctionExpressionNode* Create(Context* ctx);
            static FunctionExpressionNode* TryParse(Context* ctx);

            ParameterListNode* parameters;
            TypeSpecifierNode* returnType;
            StatementBlockNode* body;

        private:
            FunctionExpressionNode(Context* ctx);
    };
};
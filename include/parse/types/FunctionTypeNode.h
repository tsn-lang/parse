#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class FunctionTypeNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static FunctionTypeNode* Create(Context* ctx);
            static FunctionTypeNode* TryParse(Context* ctx);

            ParameterListNode* parameters;
            TypeSpecifierNode* returnType;

        private:
            FunctionTypeNode(Context* ctx);
    };
};
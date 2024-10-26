#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class FunctionNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static FunctionNode* Create(Context* ctx);
            static FunctionNode* TryParse(Context* ctx);

            bool isAsync;
            IdentifierNode* name;
            ParameterListNode* parameters;
            TypeSpecifierNode* returnType;
            StatementBlockNode* body;
            TypeParameterListNode* typeParameters;

        private:
            FunctionNode(Context* ctx);
    };
};
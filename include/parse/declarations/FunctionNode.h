#pragma once
#include <parse/misc/Node.h>
#include <utils/String.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class FunctionNode : public Node {
        public:
            virtual ~FunctionNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static FunctionNode* Create(Context* ctx);
            static FunctionNode* TryParse(Context* ctx);

            bool isAsync;
            String name;
            Array<TypedAssignableNode*> parameters;
            TypeSpecifierNode* returnType;
            StatementBlockNode* body;
            TypeParameterListNode* typeParameters;

        private:
            FunctionNode(Context* ctx);
    };
};
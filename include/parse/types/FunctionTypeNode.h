#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class FunctionTypeNode : public Node {
        public:
            virtual ~FunctionTypeNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static FunctionTypeNode* Create(Context* ctx);
            static FunctionTypeNode* TryParse(Context* ctx);

            Array<TypedAssignableNode*> parameters;
            TypeSpecifierNode* returnType;

        private:
            FunctionTypeNode(Context* ctx);
    };
};
#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class TypeParameterListNode : public Node {
        public:
            virtual ~TypeParameterListNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static TypeParameterListNode* Create(Context* ctx);
            static TypeParameterListNode* TryParse(Context* ctx);

            Array<TypeParameterNode*> parameters;

        private:
            TypeParameterListNode(Context* ctx);
    };
};
#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class ParameterListNode : public Node {
        public:
            virtual ~ParameterListNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ParameterListNode* Create(Context* ctx);
            static ParameterListNode* TryParse(Context* ctx);

            Array<TypedAssignableNode*> parameters;

        private:
            ParameterListNode(Context* ctx);
    };
};
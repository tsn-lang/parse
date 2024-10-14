#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class ObjectTypeNode : public Node {
        public:
            virtual ~ObjectTypeNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ObjectTypeNode* Create(Context* ctx);
            static ObjectTypeNode* TryParse(Context* ctx);

            Array<TypedAssignableNode*> properties;

        private:
            ObjectTypeNode(Context* ctx);
    };
};
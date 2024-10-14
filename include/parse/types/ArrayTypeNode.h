#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class ArrayTypeNode : public Node {
        public:
            virtual ~ArrayTypeNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ArrayTypeNode* Create(Context* ctx);
            static ArrayTypeNode* TryParse(Context* ctx);

            Array<TypedAssignableNode*> elements;

        private:
            ArrayTypeNode(Context* ctx);
    };
};
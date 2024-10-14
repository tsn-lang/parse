#pragma once
#include <parse/misc/Node.h>
#include <utils/String.h>

namespace parse {
    class Context;

    class TypedAssignableNode : public Node {
        public:
            virtual ~TypedAssignableNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static TypedAssignableNode* Create(Context* ctx);
            static TypedAssignableNode* TryParse(Context* ctx);

            String name;
            TypeSpecifierNode* type;

        private:
            TypedAssignableNode(Context* ctx);
    };
};
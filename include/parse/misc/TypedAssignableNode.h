#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class TypedAssignableNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static TypedAssignableNode* Create(Context* ctx);
            static TypedAssignableNode* TryParse(Context* ctx);

            IdentifierNode* name;
            TypeSpecifierNode* type;

        private:
            TypedAssignableNode(Context* ctx);
    };
};
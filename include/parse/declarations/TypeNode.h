#pragma once
#include <parse/misc/Node.h>
#include <utils/String.h>

namespace parse {
    class Context;

    class TypeNode : public Node {
        public:
            virtual ~TypeNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static TypeNode* Create(Context* ctx);
            static TypeNode* TryParse(Context* ctx);

            String name;
            TypeSpecifierNode* type;
            TypeParameterListNode* parameters;

        private:
            TypeNode(Context* ctx);
    };
};
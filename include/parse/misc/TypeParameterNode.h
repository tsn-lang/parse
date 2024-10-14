#pragma once
#include <parse/misc/Node.h>
#include <utils/String.h>

namespace parse {
    class Context;

    class TypeParameterNode : public Node {
        public:
            virtual ~TypeParameterNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static TypeParameterNode* Create(Context* ctx);
            static TypeParameterNode* TryParse(Context* ctx);

            String name;
            // todo: constraints

        private:
            TypeParameterNode(Context* ctx);
    };
};
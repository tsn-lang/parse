#pragma once
#include <parse/Node.h>

namespace parse {
    class Context;

    class TypeNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static TypeNode* Create(Context* ctx);
            static TypeNode* TryParse(Context* ctx);

        private:
            TypeNode(Context* ctx);

    };
};
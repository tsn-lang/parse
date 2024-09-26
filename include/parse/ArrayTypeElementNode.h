#pragma once
#include <parse/Node.h>

namespace parse {
    class Context;

    class ArrayTypeElementNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ArrayTypeElementNode* Create(Context* ctx);
            static ArrayTypeElementNode* TryParse(Context* ctx);

        private:
            ArrayTypeElementNode(Context* ctx);
    };
};
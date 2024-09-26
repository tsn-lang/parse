#pragma once
#include <parse/Node.h>

namespace parse {
    class Context;

    class ArrayTypeNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ArrayTypeNode* Create(Context* ctx);
            static ArrayTypeNode* TryParse(Context* ctx);

        private:
            ArrayTypeNode(Context* ctx);
    };
};
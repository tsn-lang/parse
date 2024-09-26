#pragma once
#include <parse/Node.h>

namespace parse {
    class Context;

    class ObjectTypeNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ObjectTypeNode* Create(Context* ctx);
            static ObjectTypeNode* TryParse(Context* ctx);

        private:
            ObjectTypeNode(Context* ctx);
    };
};
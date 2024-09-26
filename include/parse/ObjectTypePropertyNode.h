#pragma once
#include <parse/Node.h>

namespace parse {
    class Context;

    class ObjectTypePropertyNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ObjectTypePropertyNode* Create(Context* ctx);
            static ObjectTypePropertyNode* TryParse(Context* ctx);

        private:
            ObjectTypePropertyNode(Context* ctx);
    };
};
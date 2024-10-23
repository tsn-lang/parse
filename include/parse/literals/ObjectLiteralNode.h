#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class ObjectLiteralNode : public Node {
        public:
            virtual ~ObjectLiteralNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ObjectLiteralNode* Create(Context* ctx);
            static ObjectLiteralNode* TryParse(Context* ctx);

            Array<ObjectLiteralPropertyNode*> properties;

        private:
            ObjectLiteralNode(Context* ctx);
    };
};
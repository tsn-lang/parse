#pragma once
#include <parse/misc/Node.h>
#include <utils/String.h>

namespace parse {
    class Context;

    class ObjectLiteralPropertyNode : public Node {
        public:
            virtual ~ObjectLiteralPropertyNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ObjectLiteralPropertyNode* Create(Context* ctx);
            static ObjectLiteralPropertyNode* TryParse(Context* ctx);

            String name;
            ExpressionNode* value;

        private:
            ObjectLiteralPropertyNode(Context* ctx);
    };
};
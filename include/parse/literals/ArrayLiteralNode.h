#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class ArrayLiteralNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ArrayLiteralNode* Create(Context* ctx);
            static ArrayLiteralNode* TryParse(Context* ctx);

            ExpressionSequenceNode* elements;

        private:
            ArrayLiteralNode(Context* ctx);
    };
};
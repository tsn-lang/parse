#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class NullLiteralNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static NullLiteralNode* Create(Context* ctx);

        private:
            NullLiteralNode(Context* ctx);
    };
};
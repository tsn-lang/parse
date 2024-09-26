#pragma once
#include <parse/Node.h>

namespace parse {
    class Context;

    class ErrorNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ErrorNode* Create(Context* ctx);

        private:
            ErrorNode(Context* ctx);
    };
};
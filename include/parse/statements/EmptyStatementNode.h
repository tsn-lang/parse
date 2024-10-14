#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class EmptyStatementNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static EmptyStatementNode* Create(Context* ctx);

        private:
            EmptyStatementNode(Context* ctx);
    };
};
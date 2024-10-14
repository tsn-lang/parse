#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class BreakStatementNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static BreakStatementNode* Create(Context* ctx);

        private:
            BreakStatementNode(Context* ctx);
    };
};
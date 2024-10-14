#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class ContinueStatementNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ContinueStatementNode* Create(Context* ctx);

        private:
            ContinueStatementNode(Context* ctx);
    };
};
#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class CatchStatementNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static CatchStatementNode* Create(Context* ctx);
            static CatchStatementNode* TryParse(Context* ctx);

            TypedAssignableNode* exception;
            StatementNode* body;

        private:
            CatchStatementNode(Context* ctx);
    };
};
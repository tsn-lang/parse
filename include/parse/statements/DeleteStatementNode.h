#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class DeleteStatementNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static DeleteStatementNode* Create(Context* ctx);
            static DeleteStatementNode* TryParse(Context* ctx);

            Node* expr;

        private:
            DeleteStatementNode(Context* ctx);
    };
};
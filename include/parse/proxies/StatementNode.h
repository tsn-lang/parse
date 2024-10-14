#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class StatementNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static StatementNode* Create(Context* ctx);
            static StatementNode* TryParse(Context* ctx);

            Node* stmt;

        private:
            StatementNode(Context* ctx);
    };
};
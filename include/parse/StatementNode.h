#pragma once
#include <parse/Node.h>

namespace parse {
    class Context;

    class StatementNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static StatementNode* Create(Context* ctx);
            static StatementNode* TryParse(Context* ctx);

        private:
            StatementNode(Context* ctx);

    };
};
#pragma once
#include <parse/Node.h>

namespace parse {
    class Context;

    class StatementBlockNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static StatementBlockNode* Create(Context* ctx);
            static StatementBlockNode* TryParse(Context* ctx);

        private:
            StatementBlockNode(Context* ctx);
    };
};
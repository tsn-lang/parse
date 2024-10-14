#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class StatementBlockNode : public Node {
        public:
            virtual ~StatementBlockNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static StatementBlockNode* Create(Context* ctx);
            static StatementBlockNode* TryParse(Context* ctx);

            Array<StatementNode*> statements;

        private:
            StatementBlockNode(Context* ctx);
    };
};
#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class DoWhileLoopNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static DoWhileLoopNode* Create(Context* ctx);
            static DoWhileLoopNode* TryParse(Context* ctx);

            StatementNode* body;
            ExpressionNode* condition;

        private:
            DoWhileLoopNode(Context* ctx);
    };
};
#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class WhileLoopNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static WhileLoopNode* Create(Context* ctx);
            static WhileLoopNode* TryParse(Context* ctx);

            StatementNode* body;
            ExpressionNode* condition;

        private:
            WhileLoopNode(Context* ctx);
    };
};
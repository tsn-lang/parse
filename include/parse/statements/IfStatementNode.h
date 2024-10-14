#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class IfStatementNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static IfStatementNode* Create(Context* ctx);
            static IfStatementNode* TryParse(Context* ctx);

            ExpressionNode* condition;
            StatementNode* body;
            StatementNode* elseBody;
        private:
            IfStatementNode(Context* ctx);
    };
};
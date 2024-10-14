#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class ExpressionStatementNode : public Node {
        public:
            virtual ~ExpressionStatementNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ExpressionStatementNode* Create(Context* ctx);
            static ExpressionStatementNode* TryParse(Context* ctx);

            Array<ExpressionNode*> expressions;

        private:
            ExpressionStatementNode(Context* ctx);
    };
};
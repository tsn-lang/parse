#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class ReturnStatementNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ReturnStatementNode* Create(Context* ctx);
            static ReturnStatementNode* TryParse(Context* ctx);

            ExpressionNode* returnValue;
            
        private:
            ReturnStatementNode(Context* ctx);
    };
};
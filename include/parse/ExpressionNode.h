#pragma once
#include <parse/Node.h>

namespace parse {
    class Context;

    class ExpressionNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ExpressionNode* Create(Context* ctx);
            static ExpressionNode* TryParse(Context* ctx);

        private:
            ExpressionNode(Context* ctx);
    };
};
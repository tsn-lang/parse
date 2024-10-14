#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class ThisExpressionNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ThisExpressionNode* Create(Context* ctx);
            static ThisExpressionNode* TryParse(Context* ctx);

        private:
            ThisExpressionNode(Context* ctx);
    };
};
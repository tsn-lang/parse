#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class ThrowStatementNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ThrowStatementNode* Create(Context* ctx);
            static ThrowStatementNode* TryParse(Context* ctx);

            ExpressionNode* value;

        private:
            ThrowStatementNode(Context* ctx);
    };
};
#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class SwitchCaseNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static SwitchCaseNode* Create(Context* ctx);
            static SwitchCaseNode* TryParse(Context* ctx);

            /** null value here indicates default case */
            ExpressionNode* value;

            /** null value here indicates fallthrough */
            StatementNode* body;

        private:
            SwitchCaseNode(Context* ctx);
    };
};
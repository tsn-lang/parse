#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class SwitchStatementNode : public Node {
        public:
            virtual ~SwitchStatementNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static SwitchStatementNode* Create(Context* ctx);
            static SwitchStatementNode* TryParse(Context* ctx);

            ExpressionNode* value;
            Array<SwitchCaseNode*> cases;

        private:
            SwitchStatementNode(Context* ctx);
    };
};
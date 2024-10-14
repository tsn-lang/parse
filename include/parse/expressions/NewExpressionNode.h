#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class NewExpressionNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static NewExpressionNode* Create(Context* ctx);
            static Node* TryParse(Context* ctx);

            TypeSpecifierNode* type;
            ExpressionSequenceNode* arguments;
            Node* destination;

        private:
            NewExpressionNode(Context* ctx);
    };
};
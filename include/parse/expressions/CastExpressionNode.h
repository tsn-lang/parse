#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class CastExpressionNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static CastExpressionNode* Create(Context* ctx);
            static Node* TryParse(Context* ctx);

            Node* expr;
            TypeSpecifierNode* asType;

        private:
            CastExpressionNode(Context* ctx);
    };
};
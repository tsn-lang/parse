#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class SizeOfExpressionNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static SizeOfExpressionNode* Create(Context* ctx);
            static SizeOfExpressionNode* TryParse(Context* ctx);

            TypeSpecifierNode* type;

        private:
            SizeOfExpressionNode(Context* ctx);
    };
};
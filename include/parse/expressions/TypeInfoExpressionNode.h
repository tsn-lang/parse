#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class TypeInfoExpressionNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static TypeInfoExpressionNode* Create(Context* ctx);
            static TypeInfoExpressionNode* TryParse(Context* ctx);

            TypeSpecifierNode* type;

        private:
            TypeInfoExpressionNode(Context* ctx);
    };
};
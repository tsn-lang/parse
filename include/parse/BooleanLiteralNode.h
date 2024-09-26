#pragma once
#include <parse/Node.h>

namespace parse {
    class Context;

    class BooleanLiteralNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static BooleanLiteralNode* Create(Context* ctx);
            static BooleanLiteralNode* TryParse(Context* ctx);

            bool value;
            
        private:
            BooleanLiteralNode(Context* ctx);
    };
};
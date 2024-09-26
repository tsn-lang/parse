#pragma once
#include <parse/Node.h>
#include <utils/String.h>

namespace parse {
    class Context;

    class StringLiteralNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static StringLiteralNode* Create(Context* ctx);
            static StringLiteralNode* TryParse(Context* ctx);

            String value;
            
        private:
            StringLiteralNode(Context* ctx);
    };
};
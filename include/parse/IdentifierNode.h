#pragma once
#include <parse/Node.h>
#include <utils/String.h>

namespace parse {
    class Context;

    class IdentifierNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static IdentifierNode* Create(Context* ctx);
            static IdentifierNode* TryParse(Context* ctx);

            String text;
        private:
            IdentifierNode(Context* ctx);
    };
};
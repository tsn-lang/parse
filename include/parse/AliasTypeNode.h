#pragma once
#include <parse/Node.h>

namespace parse {
    class Context;

    class AliasTypeNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static AliasTypeNode* Create(Context* ctx);
            static AliasTypeNode* TryParse(Context* ctx);

        private:
            AliasTypeNode(Context* ctx);
    };
};
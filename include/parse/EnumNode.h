#pragma once
#include <parse/Node.h>

namespace parse {
    class Context;

    class EnumNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static EnumNode* Create(Context* ctx);
            static EnumNode* TryParse(Context* ctx);

        private:
            EnumNode(Context* ctx);

    };
};
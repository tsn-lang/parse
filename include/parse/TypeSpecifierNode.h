#pragma once
#include <parse/Node.h>

namespace parse {
    class Context;

    class TypeSpecifierNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static TypeSpecifierNode* Create(Context* ctx);
            static TypeSpecifierNode* TryParse(Context* ctx);

        private:
            TypeSpecifierNode(Context* ctx);
    };
};
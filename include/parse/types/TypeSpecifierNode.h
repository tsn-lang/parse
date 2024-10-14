#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class TypeSpecifierNode : public Node {
        public:
            enum class Modifier {
                None,
                Array,
                Pointer
            };

            virtual void acceptVisitor(INodeVisitor* visitor);
            static TypeSpecifierNode* Create(Context* ctx);
            static TypeSpecifierNode* TryParse(Context* ctx);

            Node* type;
            Modifier mod;

        private:
            TypeSpecifierNode(Context* ctx);
    };
};
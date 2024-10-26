#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class IdentifierTypeSpecifierNode : public Node {
        public:
            virtual ~IdentifierTypeSpecifierNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static IdentifierTypeSpecifierNode* Create(Context* ctx);
            static IdentifierTypeSpecifierNode* TryParse(Context* ctx);

            IdentifierNode* name;
            Array<TypeSpecifierNode*> parameters;

        private:
            IdentifierTypeSpecifierNode(Context* ctx);
    };
};
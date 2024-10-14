#pragma once
#include <parse/misc/Node.h>
#include <utils/String.h>

namespace parse {
    class Context;

    class EnumFieldNode : public Node {
        public:
            virtual ~EnumFieldNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static EnumFieldNode* Create(Context* ctx);
            static EnumFieldNode* TryParse(Context* ctx);

            String name;
            NumberLiteralNode* value;

        private:
            EnumFieldNode(Context* ctx);
    };
};
#pragma once
#include <parse/misc/Node.h>
#include <utils/String.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class EnumNode : public Node {
        public:
            virtual ~EnumNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static EnumNode* Create(Context* ctx);
            static EnumNode* TryParse(Context* ctx);

            String name;
            Array<EnumFieldNode*> fields;
            
        private:
            EnumNode(Context* ctx);
    };
};
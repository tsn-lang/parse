#pragma once
#include <parse/Node.h>
#include <utils/String.h>

namespace parse {
    class Context;

    class ClassPropertyNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ClassPropertyNode* Create(Context* ctx);
            static ClassPropertyNode* TryParse(Context* ctx);

            bool isPublic;
            bool isStatic;
            TypeSpecifierNode* type;
            String name;

        private:
            ClassPropertyNode(Context* ctx);
    };
};
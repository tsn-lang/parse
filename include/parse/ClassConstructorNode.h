#pragma once
#include <parse/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class ClassConstructorNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ClassConstructorNode* Create(Context* ctx);
            static ClassConstructorNode* TryParse(Context* ctx);

            bool isPublic;
            StatementBlockNode* body;
            Array<TypedAssignableNode*> parameters;

        private:
            ClassConstructorNode(Context* ctx);
    };
};
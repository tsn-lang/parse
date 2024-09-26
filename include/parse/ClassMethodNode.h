#pragma once
#include <parse/Node.h>
#include <utils/Array.h>
#include <utils/String.h>

namespace parse {
    class Context;

    class ClassMethodNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ClassMethodNode* Create(Context* ctx);
            static ClassMethodNode* TryParse(Context* ctx);

            bool isAsync;
            bool isStatic;
            bool isPublic;
            String name;
            Array<TypedAssignableNode*> parameters;
            TypeSpecifierNode* returnType;
            StatementBlockNode* body;

        private:
            ClassMethodNode(Context* ctx);
    };
};
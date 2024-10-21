#pragma once
#include <parse/misc/Node.h>
#include <utils/String.h>

namespace parse {
    class Context;

    class ClassMethodNode : public Node {
        public:
            virtual ~ClassMethodNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ClassMethodNode* Create(Context* ctx);
            static ClassMethodNode* TryParse(Context* ctx);

            bool isAsync;
            bool isStatic;
            bool isPublic;
            String name;
            ParameterListNode* parameters;
            TypeSpecifierNode* returnType;
            StatementBlockNode* body;
            TypeParameterListNode* typeParameters;

        private:
            ClassMethodNode(Context* ctx);
    };
};
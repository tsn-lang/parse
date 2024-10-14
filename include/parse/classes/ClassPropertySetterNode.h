#pragma once
#include <parse/misc/Node.h>
#include <utils/String.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class ClassPropertySetterNode : public Node {
        public:
            virtual ~ClassPropertySetterNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ClassPropertySetterNode* Create(Context* ctx);
            static ClassPropertySetterNode* TryParse(Context* ctx);

            bool isAsync;
            bool isStatic;
            bool isPublic;
            String propertyName;
            TypedAssignableNode* parameter;
            TypeSpecifierNode* returnType;
            StatementBlockNode* body;
            
            // this should contain exactly one node
            Array<TypedAssignableNode*> parameters;
            TypeParameterListNode* typeParameters;

        private:
            ClassPropertySetterNode(Context* ctx);
    };
};
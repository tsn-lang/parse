#pragma once
#include <parse/Node.h>
#include <utils/String.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class ClassPropertySetterNode : public Node {
        public:
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

        private:
            ClassPropertySetterNode(Context* ctx);
    };
};
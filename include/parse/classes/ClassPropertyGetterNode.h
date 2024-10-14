#pragma once
#include <parse/misc/Node.h>
#include <utils/String.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class ClassPropertyGetterNode : public Node {
        public:
            virtual ~ClassPropertyGetterNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ClassPropertyGetterNode* Create(Context* ctx);
            static ClassPropertyGetterNode* TryParse(Context* ctx);

            bool isAsync;
            bool isStatic;
            bool isPublic;
            String propertyName;
            TypeSpecifierNode* returnType;
            StatementBlockNode* body;
            
            // this should be empty
            Array<TypedAssignableNode*> parameters;

        private:
            ClassPropertyGetterNode(Context* ctx);
    };
};
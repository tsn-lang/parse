#pragma once
#include <parse/misc/Node.h>
#include <utils/String.h>

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
            ParameterListNode* parameters;

        private:
            ClassPropertyGetterNode(Context* ctx);
    };
};
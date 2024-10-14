#pragma once
#include <parse/misc/Node.h>
#include <utils/String.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class ClassOperatorMethodNode : public Node {
        public:
            virtual ~ClassOperatorMethodNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ClassOperatorMethodNode* Create(Context* ctx);
            static ClassOperatorMethodNode* TryParse(Context* ctx);

            bool isAsync;
            bool isStatic;
            bool isPublic;
            OperatorType operation;
            TypeSpecifierNode* returnType;
            StatementBlockNode* body;
            
            // This should contain either 0 or 1 parameter, depending on the operator
            Array<TypedAssignableNode*> parameters;
            TypeParameterListNode* typeParameters;

        private:
            ClassOperatorMethodNode(Context* ctx);
    };
};
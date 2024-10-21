#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>
#include <utils/String.h>

namespace parse {
    class Context;

    class ClassNode : public Node {
        public:
            virtual ~ClassNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ClassNode* Create(Context* ctx);
            static ClassNode* TryParse(Context* ctx);

            String name;
            bool isDeclaration;
            Array<TypeSpecifierNode*> extends;
            Array<ClassPropertyNode*> properties;
            Array<ClassMethodNode*> methods;
            Array<ClassOperatorMethodNode*> operators;
            Array<ClassPropertyGetterNode*> getters;
            Array<ClassPropertySetterNode*> setters;
            Array<ClassConstructorNode*> ctors;
            ClassDestructorNode* dtor;
            TypeParameterListNode* typeParameters;
            
        private:
            ClassNode(Context* ctx);
    };
};
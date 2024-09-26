#pragma once
#include <parse/Node.h>
#include <utils/Array.h>
#include <utils/String.h>

namespace parse {
    class Context;

    class ClassNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ClassNode* Create(Context* ctx);
            static ClassNode* TryParse(Context* ctx);

            String name;
            Array<TypeSpecifierNode*> extends;
            Array<ClassPropertyNode*> properties;
            Array<ClassMethodNode*> methods;
            Array<ClassOperatorMethodNode*> operators;
            Array<ClassPropertyGetterNode*> getters;
            Array<ClassPropertySetterNode*> setters;
            Array<ClassConstructorNode*> ctors;
            ClassDestructorNode* dtor;
            
        private:
            ClassNode(Context* ctx);
    };
};
#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class ClassDestructorNode : public Node {
        public:
            virtual ~ClassDestructorNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ClassDestructorNode* Create(Context* ctx);
            static ClassDestructorNode* TryParse(Context* ctx);

            bool isPublic;
            StatementBlockNode* body;
            
            // this should be empty
            Array<TypedAssignableNode*> parameters;

        private:
            ClassDestructorNode(Context* ctx);
    };
};
#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class ClassConstructorNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ClassConstructorNode* Create(Context* ctx);
            static ClassConstructorNode* TryParse(Context* ctx);

            bool isPublic;
            StatementBlockNode* body;
            ParameterListNode* parameters;

        private:
            ClassConstructorNode(Context* ctx);
    };
};
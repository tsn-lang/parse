#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class DeclarationStatementNode : public Node {
        public:
            virtual ~DeclarationStatementNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static DeclarationStatementNode* Create(Context* ctx);
            static DeclarationStatementNode* TryParse(Context* ctx);

            bool isConst;
            bool isArrayDestructure;
            bool isObjectDestructure;
            IdentifierNode* assignable;
            TypedAssignableNode* typedAssignable;
            Array<IdentifierNode*> destructureTargets;
            ExpressionNode* initializer;

        private:
            DeclarationStatementNode(Context* ctx);
    };
};
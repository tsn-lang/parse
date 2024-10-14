#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class ForLoopNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ForLoopNode* Create(Context* ctx);
            static ForLoopNode* TryParse(Context* ctx);

            DeclarationStatementNode* initializer;
            ExpressionNode* condition;
            ExpressionNode* modifier;
            StatementNode* body;

        private:
            ForLoopNode(Context* ctx);
    };
};
#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class ExpressionSequenceNode : public Node {
        public:
            virtual ~ExpressionSequenceNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ExpressionSequenceNode* Create(Context* ctx);
            static ExpressionSequenceNode* TryParse(Context* ctx);
            static ExpressionSequenceNode* TryParseParenthesized(Context* ctx);
            
            Array<ExpressionNode*> expressions;

        private:
            ExpressionSequenceNode(Context* ctx);
    };
};
#pragma once
#include <parse/misc/Node.h>
#include <utils/String.h>

namespace parse {
    class Context;

    class MemberExpressionNode : public Node {
        public:
            virtual ~MemberExpressionNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static MemberExpressionNode* Create(Context* ctx);
            static Node* TryParse(Context* ctx);
            static MemberExpressionNode* TryParseTail(Context* ctx, Node* base);

            Node* object;
            String propertyName;

        private:
            MemberExpressionNode(Context* ctx);
    };
};
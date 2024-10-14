#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class IndexExpressionNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static IndexExpressionNode* Create(Context* ctx);
            static Node* TryParse(Context* ctx);
            static IndexExpressionNode* TryParseTail(Context* ctx, Node* base);
            
            Node* object;
            Node* index;

        private:
            IndexExpressionNode(Context* ctx);
    };
};
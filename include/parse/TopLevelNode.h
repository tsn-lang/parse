#pragma once
#include <parse/Node.h>

namespace parse {
    class Context;

    class TopLevelNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static TopLevelNode* Create(Context* ctx);
            static TopLevelNode* TryParse(Context* ctx);

            Node* statement;
            
        private:
            TopLevelNode(Context* ctx);
    };
};
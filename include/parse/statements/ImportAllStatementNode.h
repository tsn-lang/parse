#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class ImportAllStatementNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ImportAllStatementNode* Create(Context* ctx);
            static ImportAllStatementNode* TryParse(Context* ctx);

            IdentifierNode* alias;
            StringLiteralNode* moduleId;
            
        private:
            ImportAllStatementNode(Context* ctx);
    };
};
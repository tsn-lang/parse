#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class ImportSelectStatementNode : public Node {
        public:
            virtual ~ImportSelectStatementNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ImportSelectStatementNode* Create(Context* ctx);
            static ImportSelectStatementNode* TryParse(Context* ctx);

            Array<IdentifierNode*> importList;
            StringLiteralNode* moduleId;

        private:
            ImportSelectStatementNode(Context* ctx);
    };
};
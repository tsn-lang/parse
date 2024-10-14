#pragma once
#include <parse/misc/Node.h>
#include <utils/String.h>
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
            String moduleId;

        private:
            ImportSelectStatementNode(Context* ctx);
    };
};
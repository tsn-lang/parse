#pragma once
#include <parse/misc/Node.h>
#include <utils/String.h>

namespace parse {
    class Context;

    class ImportAllStatementNode : public Node {
        public:
            virtual ~ImportAllStatementNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ImportAllStatementNode* Create(Context* ctx);
            static ImportAllStatementNode* TryParse(Context* ctx);

            String alias;
            String moduleId;
            
        private:
            ImportAllStatementNode(Context* ctx);
    };
};
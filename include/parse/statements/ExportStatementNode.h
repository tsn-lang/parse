#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class ExportStatementNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ExportStatementNode* Create(Context* ctx);
            static ExportStatementNode* TryParse(Context* ctx);

            Node* exportable;
        private:
            ExportStatementNode(Context* ctx);
    };
};
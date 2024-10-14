#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class TryCatchStatementNode : public Node {
        public:
            virtual ~TryCatchStatementNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static TryCatchStatementNode* Create(Context* ctx);
            static TryCatchStatementNode* TryParse(Context* ctx);

            StatementNode* tryBody;
            Array<CatchStatementNode*> catchBlocks;

        private:
            TryCatchStatementNode(Context* ctx);
    };
};
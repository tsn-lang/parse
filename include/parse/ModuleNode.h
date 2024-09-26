#pragma once
#include <parse/Node.h>
#include <utils/Array.h>

namespace parse {
    class Context;

    class ModuleNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static ModuleNode* Create(Context* ctx);
            static ModuleNode* TryParse(Context* ctx);

            Array<TopLevelNode*> statements;
        
        private:
            ModuleNode(Context* ctx);
    };
};
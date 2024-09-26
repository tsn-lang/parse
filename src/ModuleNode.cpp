#include <parse/ModuleNode.h>
#include <parse/Context.h>
#include <parse/TopLevelNode.h>
#include <utils/Array.hpp>

namespace parse {
    ModuleNode::ModuleNode(Context* ctx) : Node(ctx) {}
    void ModuleNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ModuleNode* ModuleNode::Create(Context* ctx) { return new (ctx->allocNode()) ModuleNode(ctx); }

    ModuleNode* ModuleNode::TryParse(Context* ctx) {
        ModuleNode* out = Create(ctx);

        while (!ctx->atEnd()) {
            TopLevelNode* n = TopLevelNode::TryParse(ctx);
            if (!n || n->isError()) {
                if (n) n->destroy();
                ctx->skipTo(TokenType::EndOfStatement);
                continue;
            }

            out->extendLocation(n);
            out->statements.push(n);
        }

        return out;
    }
};
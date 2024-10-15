#include <parse/misc/ModuleNode.h>
#include <parse/proxies/TopLevelNode.h>
#include <parse/Context.h>
#include <utils/Array.hpp>

namespace parse {
    ModuleNode::ModuleNode(Context* ctx) : Node(ctx, NodeType::ModuleNode) {}
    ModuleNode::~ModuleNode() {}
    void ModuleNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ModuleNode* ModuleNode::Create(Context* ctx) { return new (ctx->allocNode()) ModuleNode(ctx); }

    ModuleNode* ModuleNode::TryParse(Context* ctx) {
        ModuleNode* out = Create(ctx);

        while (!ctx->atEnd()) {
            TopLevelNode* n = TopLevelNode::TryParse(ctx);
            if (!n) {
                ctx->skipTo(TokenType::EndOfStatement);
                continue;
            }

            out->extendLocation(n);
            out->statements.push(n);
        }

        return out;
    }
};
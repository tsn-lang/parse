#include <parse/statements/ExportStatementNode.h>
#include <parse/declarations/ClassNode.h>
#include <parse/declarations/FunctionNode.h>
#include <parse/declarations/TypeNode.h>
#include <parse/statements/DeclarationStatementNode.h>
#include <parse/misc/IdentifierNode.h>
#include <parse/Context.h>

namespace parse {
    ExportStatementNode::ExportStatementNode(Context* ctx) : Node(ctx, NodeType::ExportStatementNode), exportable(nullptr) {}
    void ExportStatementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ExportStatementNode* ExportStatementNode::Create(Context* ctx) { return new (ctx->allocNode()) ExportStatementNode(ctx); }

    ExportStatementNode* ExportStatementNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Export)) return nullptr;

        ExportStatementNode* n = Create(ctx);
        ctx->consume(n);

        n->exportable = IdentifierNode::TryParse(ctx);
        if (n->exportable) {
            n->extendLocation(n->exportable);
            return n;
        }

        n->exportable = ClassNode::TryParse(ctx);
        if (n->exportable) {
            n->extendLocation(n->exportable);
            return n;
        }
        
        n->exportable = FunctionNode::TryParse(ctx);
        if (n->exportable) {
            n->extendLocation(n->exportable);
            return n;
        }

        n->exportable = TypeNode::TryParse(ctx);
        if (n->exportable) {
            n->extendLocation(n->exportable);
            return n;
        }

        n->exportable = DeclarationStatementNode::TryParse(ctx);
        if (n->exportable) {
            n->extendLocation(n->exportable);
            return n;
        }

        if (!n->exportable) {
            ctx->logError("Expected valid exportable entity");
            n->m_isError = true;
            return n;
        }

        return n;
    }
};
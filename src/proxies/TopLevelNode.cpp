#include <parse/proxies/TopLevelNode.h>
#include <parse/proxies/StatementNode.h>
#include <parse/declarations/FunctionNode.h>
#include <parse/declarations/ClassNode.h>
#include <parse/declarations/TypeNode.h>
#include <parse/declarations/EnumNode.h>
#include <parse/statements/ImportAllStatementNode.h>
#include <parse/statements/ImportSelectStatementNode.h>
#include <parse/statements/ExportStatementNode.h>
#include <parse/Context.h>

namespace parse {
    TopLevelNode::TopLevelNode(Context* ctx) : Node(ctx, NodeType::TopLevelNode) {
        statement = nullptr;
    }

    void TopLevelNode::acceptVisitor(INodeVisitor* visitor) {
        if (statement) statement->acceptVisitor(visitor);
    }

    TopLevelNode* TopLevelNode::Create(Context* ctx) { return new (ctx->allocNode()) TopLevelNode(ctx); }

    TopLevelNode* TopLevelNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        FunctionNode* fdecl = FunctionNode::TryParse(ctx);
        if (fdecl) {
            TopLevelNode* tl = Create(ctx);
            tl->statement = fdecl;
            tl->m_isError = tl->statement->isError();
            tl->m_location = tl->statement->getLocation();
            return tl;
        }

        ClassNode* cdecl_ = ClassNode::TryParse(ctx);
        if (cdecl_) {
            TopLevelNode* tl = Create(ctx);
            tl->statement = cdecl_;
            tl->m_isError = tl->statement->isError();
            tl->m_location = tl->statement->getLocation();
            return tl;
        }

        TypeNode* tdecl = TypeNode::TryParse(ctx);
        if (tdecl) {
            TopLevelNode* tl = Create(ctx);
            tl->statement = tdecl;
            tl->m_isError = tl->statement->isError();
            tl->m_location = tl->statement->getLocation();
            return tl;
        }
        
        EnumNode* edecl = EnumNode::TryParse(ctx);
        if (edecl) {
            TopLevelNode* tl = Create(ctx);
            tl->statement = edecl;
            tl->m_isError = tl->statement->isError();
            tl->m_location = tl->statement->getLocation();
            return tl;
        }

        ExportStatementNode* exportStmt = ExportStatementNode::TryParse(ctx);
        if (exportStmt) {
            TopLevelNode* tl = Create(ctx);
            tl->statement = exportStmt;
            tl->m_isError = tl->statement->isError();
            tl->m_location = tl->statement->getLocation();
            return tl;
        }

        ImportAllStatementNode* importAll = ImportAllStatementNode::TryParse(ctx);
        if (importAll) {
            TopLevelNode* tl = Create(ctx);
            tl->statement = importAll;
            tl->m_isError = tl->statement->isError();
            tl->m_location = tl->statement->getLocation();
            return tl;
        }

        ImportSelectStatementNode* importSelect = ImportSelectStatementNode::TryParse(ctx);
        if (importSelect) {
            TopLevelNode* tl = Create(ctx);
            tl->statement = importSelect;
            tl->m_isError = tl->statement->isError();
            tl->m_location = tl->statement->getLocation();
            return tl;
        }

        StatementNode* stmt = StatementNode::TryParse(ctx);
        if (stmt) {
            TopLevelNode* tl = Create(ctx);
            tl->statement = stmt;
            tl->m_isError = tl->statement->isError();
            tl->m_location = tl->statement->getLocation();
            return tl;
        }

        return nullptr;
    }
};
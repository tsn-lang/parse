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
    TopLevelNode::TopLevelNode(Context* ctx) : Node(ctx) {
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
            tl->m_isError = fdecl->isError();
            tl->statement = fdecl;
            tl->extendLocation(fdecl);
            return tl;
        }

        ClassNode* cdecl_ = ClassNode::TryParse(ctx);
        if (cdecl_) {
            TopLevelNode* tl = Create(ctx);
            tl->m_isError = cdecl_->isError();
            tl->statement = cdecl_;
            tl->extendLocation(cdecl_);
            return tl;
        }

        TypeNode* tdecl = TypeNode::TryParse(ctx);
        if (tdecl) {
            TopLevelNode* tl = Create(ctx);
            tl->m_isError = tdecl->isError();
            tl->statement = tdecl;
            tl->extendLocation(tdecl);
            return tl;
        }
        
        EnumNode* edecl = EnumNode::TryParse(ctx);
        if (edecl) {
            TopLevelNode* tl = Create(ctx);
            tl->m_isError = edecl->isError();
            tl->statement = edecl;
            tl->extendLocation(edecl);
            return tl;
        }

        ExportStatementNode* exportStmt = ExportStatementNode::TryParse(ctx);
        if (exportStmt) {
            TopLevelNode* tl = Create(ctx);
            tl->m_isError = exportStmt->isError();
            tl->statement = exportStmt;
            tl->extendLocation(exportStmt);
            return tl;
        }

        ImportAllStatementNode* importAll = ImportAllStatementNode::TryParse(ctx);
        if (importAll) {
            TopLevelNode* tl = Create(ctx);
            tl->m_isError = importAll->isError();
            tl->statement = importAll;
            tl->extendLocation(importAll);
            return tl;
        }

        ImportSelectStatementNode* importSelect = ImportSelectStatementNode::TryParse(ctx);
        if (importSelect) {
            TopLevelNode* tl = Create(ctx);
            tl->m_isError = importSelect->isError();
            tl->statement = importSelect;
            tl->extendLocation(importSelect);
            return tl;
        }

        StatementNode* stmt = StatementNode::TryParse(ctx);
        if (stmt) {
            TopLevelNode* tl = Create(ctx);
            tl->m_isError = stmt->isError();
            tl->statement = stmt;
            tl->extendLocation(stmt);
            return tl;
        }

        return nullptr;
    }
};
#include <parse/TopLevelNode.h>
#include <parse/Context.h>
#include <parse/ErrorNode.h>
#include <parse/StatementNode.h>
#include <parse/FunctionNode.h>
#include <parse/ClassNode.h>
#include <parse/TypeNode.h>
#include <parse/EnumNode.h>

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
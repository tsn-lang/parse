#include <parse/proxies/StatementNode.h>
#include <parse/Context.h>
#include <parse/statements/BreakStatementNode.h>
#include <parse/statements/ContinueStatementNode.h>
#include <parse/statements/DeclarationStatementNode.h>
#include <parse/statements/DeleteStatementNode.h>
#include <parse/statements/DoWhileLoopNode.h>
#include <parse/statements/EmptyStatementNode.h>
#include <parse/statements/ExportStatementNode.h>
#include <parse/statements/ExpressionStatementNode.h>
#include <parse/statements/ForLoopNode.h>
#include <parse/statements/IfStatementNode.h>
#include <parse/statements/ReturnStatementNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <parse/statements/SwitchCaseNode.h>
#include <parse/statements/SwitchStatementNode.h>
#include <parse/statements/ThrowStatementNode.h>
#include <parse/statements/TryCatchStatementNode.h>
#include <parse/statements/WhileLoopNode.h>
#include <parse/proxies/ExpressionNode.h>

namespace parse {
    StatementNode::StatementNode(Context* ctx) : Node(ctx, NodeType::StatementNode), stmt(nullptr) {}
    void StatementNode::acceptVisitor(INodeVisitor* visitor) {
        if (stmt) stmt->acceptVisitor(visitor);
    
    }
    StatementNode* StatementNode::Create(Context* ctx) { return new (ctx->allocNode()) StatementNode(ctx); }

    StatementNode* StatementNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        if (ctx->match(TokenType::EndOfStatement)) {
            StatementNode* n = Create(ctx);
            n->stmt = EmptyStatementNode::Create(ctx);
            n->stmt->extendLocation(ctx->get());
            n->extendLocation(n->stmt);
            ctx->consume();
            return n;
        }

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Break)) {
            StatementNode* n = Create(ctx);
            n->stmt = BreakStatementNode::Create(ctx);
            n->stmt->extendLocation(ctx->get());
            n->extendLocation(n->stmt);
            ctx->consume();

            if (!ctx->match(TokenType::EndOfStatement)) {
                ctx->logError("Expected ';'");
                n->m_isError = true;
                return n;
            }

            ctx->consume(n);

            return n;
        }

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Continue)) {
            StatementNode* n = Create(ctx);
            n->stmt = ContinueStatementNode::Create(ctx);
            n->stmt->extendLocation(ctx->get());
            n->extendLocation(n->stmt);
            ctx->consume();

            if (!ctx->match(TokenType::EndOfStatement)) {
                ctx->logError("Expected ';'");
                n->m_isError = true;
                return n;
            }

            ctx->consume(n);

            return n;
        }

        DeleteStatementNode* delStmt = DeleteStatementNode::TryParse(ctx);
        if (delStmt) {
            StatementNode* n = Create(ctx);
            n->stmt = delStmt;
            n->m_isError = n->stmt->isError();
            n->m_location = n->stmt->getLocation();

            if (!ctx->match(TokenType::EndOfStatement)) {
                ctx->logError("Expected ';'");
                n->m_isError = true;
                return n;
            }

            ctx->consume(n);

            return n;
        }

        ReturnStatementNode* retStmt = ReturnStatementNode::TryParse(ctx);
        if (retStmt) {
            StatementNode* n = Create(ctx);
            n->stmt = retStmt;
            n->m_isError = n->stmt->isError();
            n->m_location = n->stmt->getLocation();

            if (!ctx->match(TokenType::EndOfStatement)) {
                ctx->logError("Expected ';'");
                n->m_isError = true;
                return n;
            }

            ctx->consume(n);

            return n;
        }

        ThrowStatementNode* throwStmt = ThrowStatementNode::TryParse(ctx);
        if (throwStmt) {
            StatementNode* n = Create(ctx);
            n->stmt = throwStmt;
            n->m_isError = n->stmt->isError();
            n->m_location = n->stmt->getLocation();

            if (!ctx->match(TokenType::EndOfStatement)) {
                ctx->logError("Expected ';'");
                n->m_isError = true;
                return n;
            }

            ctx->consume(n);

            return n;
        }

        DeclarationStatementNode* declStmt = DeclarationStatementNode::TryParse(ctx);
        if (declStmt) {
            StatementNode* n = Create(ctx);
            n->stmt = declStmt;
            n->m_isError = n->stmt->isError();
            n->m_location = n->stmt->getLocation();

            if (!ctx->match(TokenType::EndOfStatement)) {
                ctx->logError("Expected ';'");
                n->m_isError = true;
                return n;
            }

            ctx->consume(n);

            return n;
        }

        StatementBlockNode* blockStmt = StatementBlockNode::TryParse(ctx);
        if (blockStmt) {
            StatementNode* n = Create(ctx);
            n->stmt = blockStmt;
            n->m_isError = n->stmt->isError();
            n->m_location = n->stmt->getLocation();

            return n;
        }

        ExpressionStatementNode* exprStmt = ExpressionStatementNode::TryParse(ctx);
        if (exprStmt) {
            StatementNode* n = Create(ctx);
            n->stmt = exprStmt;
            n->m_isError = n->stmt->isError();
            n->m_location = n->stmt->getLocation();

            if (!ctx->match(TokenType::EndOfStatement)) {
                ctx->logError("Expected ';'");
                n->m_isError = true;
                return n;
            }

            ctx->consume(n);

            return n;
        }

        IfStatementNode* ifStmt = IfStatementNode::TryParse(ctx);
        if (ifStmt) {
            StatementNode* n = Create(ctx);
            n->stmt = ifStmt;
            n->m_isError = n->stmt->isError();
            n->m_location = n->stmt->getLocation();

            return n;
        }
        
        ForLoopNode* forLoop = ForLoopNode::TryParse(ctx);
        if (forLoop) {
            StatementNode* n = Create(ctx);
            n->stmt = forLoop;
            n->m_isError = n->stmt->isError();
            n->m_location = n->stmt->getLocation();

            return n;
        }
        
        SwitchStatementNode* switchStmt = SwitchStatementNode::TryParse(ctx);
        if (switchStmt) {
            StatementNode* n = Create(ctx);
            n->stmt = switchStmt;
            n->m_isError = n->stmt->isError();
            n->m_location = n->stmt->getLocation();

            return n;
        }
        
        WhileLoopNode* whileLoop = WhileLoopNode::TryParse(ctx);
        if (whileLoop) {
            StatementNode* n = Create(ctx);
            n->stmt = whileLoop;
            n->m_isError = n->stmt->isError();
            n->m_location = n->stmt->getLocation();

            return n;
        }
        
        DoWhileLoopNode* doWhileLoop = DoWhileLoopNode::TryParse(ctx);
        if (doWhileLoop) {
            StatementNode* n = Create(ctx);
            n->stmt = doWhileLoop;
            n->m_isError = n->stmt->isError();
            n->m_location = n->stmt->getLocation();

            return n;
        }

        TryCatchStatementNode* tryCatchStmt = TryCatchStatementNode::TryParse(ctx);
        if (tryCatchStmt) {
            StatementNode* n = Create(ctx);
            n->stmt = tryCatchStmt;
            n->m_isError = n->stmt->isError();
            n->m_location = n->stmt->getLocation();

            return n;
        }

        return nullptr;
    }
};
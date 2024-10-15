#include <parse/statements/StatementBlockNode.h>
#include <parse/proxies/StatementNode.h>
#include <parse/Context.h>
#include <utils/Array.hpp>

namespace parse {
    StatementBlockNode::StatementBlockNode(Context* ctx) : Node(ctx, NodeType::StatementBlockNode) {}
    StatementBlockNode::~StatementBlockNode() {}
    void StatementBlockNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    StatementBlockNode* StatementBlockNode::Create(Context* ctx) { return new (ctx->allocNode()) StatementBlockNode(ctx); }

    StatementBlockNode* StatementBlockNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenBrace)) return nullptr;

        ctx->begin();
        StatementBlockNode* n = Create(ctx);
        ctx->consume(n);

        StatementNode* stmt = StatementNode::TryParse(ctx);
        if (!stmt) {
            ctx->rollback();
            n->destroy();
            return nullptr;
        }
        
        ctx->commit();

        while (stmt) {
            if (stmt->isError()) {
                stmt->destroy();
                if (!ctx->skipTo(TokenType::EndOfStatement)) {
                    if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)) {
                        // no EOS, no block end?
                        n->m_isError = true;
                        return n;
                    }
                    
                    // end of block hopefully
                    break;
                }

                ctx->consume(n);

                stmt = StatementNode::TryParse(ctx);
                continue;
            }

            n->statements.push(stmt);
            n->extendLocation(stmt);

            if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)) break;
            
            stmt = StatementNode::TryParse(ctx);
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseBrace)) {
            ctx->logError("Expected '}'");
            n->m_isError = true;
        } else {
            ctx->consume(n);
        }

        return n;
    }
};
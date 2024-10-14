#include <parse/statements/TryCatchStatementNode.h>
#include <parse/proxies/StatementNode.h>
#include <parse/statements/CatchStatementNode.h>
#include <parse/Context.h>
#include <utils/Array.hpp>

namespace parse {
    TryCatchStatementNode::TryCatchStatementNode(Context* ctx) : Node(ctx), tryBody(nullptr) {}
    TryCatchStatementNode::~TryCatchStatementNode() {}
    void TryCatchStatementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    TryCatchStatementNode* TryCatchStatementNode::Create(Context* ctx) { return new (ctx->allocNode()) TryCatchStatementNode(ctx); }

    TryCatchStatementNode* TryCatchStatementNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Try)) return nullptr;

        TryCatchStatementNode* n = Create(ctx);
        ctx->consume(n);

        n->tryBody = StatementNode::TryParse(ctx);
        if (!n->tryBody) {
            n->m_isError = true;
            ctx->logError("Expected try statement body");
            if (!ctx->skipTo(TokenType::Keyword, TokenSubType::Keyword_Catch)) {
                return n;
            }
        }

        CatchStatementNode* catchBlock = CatchStatementNode::TryParse(ctx);
        while (catchBlock) {
            if (catchBlock->isError()) {
                n->m_isError = true;
                catchBlock->destroy();
                if (!ctx->skipTo(TokenType::Keyword, TokenSubType::Keyword_Catch)) {
                    break;
                }
            }

            n->catchBlocks.push(catchBlock);
            n->extendLocation(catchBlock);
            catchBlock = CatchStatementNode::TryParse(ctx);
        }

        return n;
    }
};
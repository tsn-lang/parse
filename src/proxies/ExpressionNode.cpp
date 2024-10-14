#include <parse/proxies/ExpressionNode.h>
#include <parse/expressions/AwaitExpressionNode.h>
#include <parse/expressions/ThisExpressionNode.h>
#include <parse/expressions/TypeInfoExpressionNode.h>
#include <parse/expressions/SizeOfExpressionNode.h>
#include <parse/expressions/ExpressionSequenceNode.h>
#include <parse/literals/BooleanLiteralNode.h>
#include <parse/literals/NullLiteralNode.h>
#include <parse/literals/NumberLiteralNode.h>
#include <parse/literals/StringLiteralNode.h>
#include <parse/literals/TemplateStringLiteralNode.h>
#include <parse/types/IdentifierTypeSpecifierNode.h>
#include <parse/misc/IdentifierNode.h>
#include <parse/Context.h>

namespace parse {
    ExpressionNode::ExpressionNode(Context* ctx) : Node(ctx), expr(nullptr) {}
    void ExpressionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ExpressionNode* ExpressionNode::Create(Context* ctx) { return new (ctx->allocNode()) ExpressionNode(ctx); }

    ExpressionNode* ExpressionNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        Node* expr = AwaitExpressionNode::TryParse(ctx);
        if (!expr) return nullptr;

        ExpressionNode* n = Create(ctx);
        n->expr = expr;
        n->extendLocation(expr);
        n->m_isError = expr->isError();

        return n;
    }

    ExpressionNode* ExpressionNode::TryParseParenthesized(Context* ctx) {
        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) return nullptr;

        ctx->begin();

        const Token* openTok = ctx->get();
        ctx->consume();

        ExpressionNode* n = TryParse(ctx);
        if (!n) {
            ctx->rollback();
            return nullptr;
        }

        n->extendLocation(openTok);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseParen)) {
            n->m_isError = true;
            ctx->logError("Expected ')'");
            return n;
        }

        ctx->consume(n);
        return n;
    }

    ExpressionNode* ExpressionNode::TryParsePrimaryExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_This)) {
            ExpressionNode* n = Create(ctx);
            n->expr = ThisExpressionNode::Create(ctx);
            ctx->consume(n->expr);
            n->extendLocation(n->expr);
            return n;
        }

        if (ctx->match(TokenType::Identifier)) {
            ctx->begin();
            IdentifierTypeSpecifierNode* ts = IdentifierTypeSpecifierNode::TryParse(ctx);
            if (ts) {
                if (!ts->isError() && ts->parameters.size() > 0) {
                    ctx->commit();
                    ExpressionNode* n = Create(ctx);
                    n->expr = ts;
                    n->extendLocation(ts);
                    return n;
                }
                
                ctx->rollback();
                ts->destroy();
            }

            ExpressionNode* n = Create(ctx);
            n->expr = IdentifierNode::TryParse(ctx);
            n->extendLocation(n->expr);
            return n;
        }

        if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) {
            ctx->begin();
            ExpressionNode* pe = ExpressionNode::TryParseParenthesized(ctx);
            if (pe && !pe->isError()) {
                ctx->commit();
                return pe;
            }

            ctx->rollback();
            if (pe) pe->destroy();

            ExpressionSequenceNode* es = ExpressionSequenceNode::TryParse(ctx);
            if (es && !es->isError()) {
                ctx->commit();
                ExpressionNode* n = Create(ctx);
                n->expr = es;
                n->extendLocation(n->expr);
                return n;
            }

            ctx->rollback();
            return nullptr;
        }

        TypeInfoExpressionNode* ti = TypeInfoExpressionNode::TryParse(ctx);
        if (ti) {
            ExpressionNode* n = Create(ctx);
            n->expr = ti;
            n->m_isError = ti->isError();
            n->extendLocation(ti);
            return n;
        }

        SizeOfExpressionNode* so = SizeOfExpressionNode::TryParse(ctx);
        if (so) {
            ExpressionNode* n = Create(ctx);
            n->expr = so;
            n->m_isError = so->isError();
            n->extendLocation(so);
            return n;
        }

        if (ctx->match(TokenType::Literal, TokenSubType::Literal_Null)) {
            ExpressionNode* n = Create(ctx);
            n->expr = NullLiteralNode::Create(ctx);
            ctx->consume(n->expr);
            n->extendLocation(n->expr);
            return n;
        }

        BooleanLiteralNode* bl = BooleanLiteralNode::TryParse(ctx);
        if (bl) {
            ExpressionNode* n = Create(ctx);
            n->expr = bl;
            n->m_isError = bl->isError();
            n->extendLocation(bl);
            return n;
        }

        NumberLiteralNode* nl = NumberLiteralNode::TryParse(ctx);
        if (nl) {
            ExpressionNode* n = Create(ctx);
            n->expr = nl;
            n->m_isError = nl->isError();
            n->extendLocation(nl);
            return n;
        }

        StringLiteralNode* sl = StringLiteralNode::TryParse(ctx);
        if (sl) {
            ExpressionNode* n = Create(ctx);
            n->expr = sl;
            n->m_isError = sl->isError();
            n->extendLocation(sl);
            return n;
        }

        TemplateStringLiteralNode* ts = TemplateStringLiteralNode::TryParse(ctx);
        if (ts) {
            ExpressionNode* n = Create(ctx);
            n->expr = ts;
            n->m_isError = ts->isError();
            n->extendLocation(ts);
            return n;
        }

        return nullptr;
    }
};
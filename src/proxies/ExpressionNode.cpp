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
    ExpressionNode::ExpressionNode(Context* ctx) : Node(ctx, NodeType::ExpressionNode), expr(nullptr) {}
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

    Node* ExpressionNode::TryParsePrimaryExpression(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_This)) {
            ThisExpressionNode* n = ThisExpressionNode::Create(ctx);
            ctx->consume(n);
            return n;
        }

        if (ctx->match(TokenType::Identifier)) {
            ctx->begin();
            IdentifierTypeSpecifierNode* ts = IdentifierTypeSpecifierNode::TryParse(ctx);
            if (ts) {
                if (!ts->isError() && ts->parameters.size() > 0) {
                    ctx->commit();
                    return ts;
                }
                
                ctx->rollback();
                ts->destroy();
            }

            return IdentifierNode::TryParse(ctx);
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

            ExpressionSequenceNode* es = ExpressionSequenceNode::TryParseParenthesized(ctx);
            if (es && !es->isError()) {
                ctx->commit();
                return es;
            }

            ctx->rollback();
            return nullptr;
        }

        TypeInfoExpressionNode* ti = TypeInfoExpressionNode::TryParse(ctx);
        if (ti) return ti;

        SizeOfExpressionNode* so = SizeOfExpressionNode::TryParse(ctx);
        if (so) return so;

        if (ctx->match(TokenType::Literal, TokenSubType::Literal_Null)) {
            NullLiteralNode* n = NullLiteralNode::Create(ctx);
            ctx->consume(n);
            return n;
        }

        BooleanLiteralNode* bl = BooleanLiteralNode::TryParse(ctx);
        if (bl) return bl;

        NumberLiteralNode* nl = NumberLiteralNode::TryParse(ctx);
        if (nl) return nl;

        StringLiteralNode* sl = StringLiteralNode::TryParse(ctx);
        if (sl) return sl;

        TemplateStringLiteralNode* ts = TemplateStringLiteralNode::TryParse(ctx);
        if (ts) return ts;

        return nullptr;
    }
};
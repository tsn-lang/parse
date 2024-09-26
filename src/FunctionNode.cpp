#include <parse/FunctionNode.h>
#include <parse/TypedAssignableNode.h>
#include <parse/TypeSpecifierNode.h>
#include <parse/StatementBlockNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>
#include <utils/Array.hpp>

namespace parse {
    FunctionNode::FunctionNode(Context* ctx) : Node(ctx), isAsync(false), body(nullptr) {}
    void FunctionNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    FunctionNode* FunctionNode::Create(Context* ctx) { return new (ctx->allocNode()) FunctionNode(ctx); }

    FunctionNode* FunctionNode::TryParse(Context* ctx) {
        if (!ctx->matchAny({
            Match(TokenType::Keyword, TokenSubType::Keyword_Function),
            Match(TokenType::Keyword, TokenSubType::Keyword_Async)
        })) return nullptr;

        FunctionNode* n = Create(ctx);

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Async)) {
            n->isAsync = true;
            n->extendLocation(ctx->get());
            ctx->consume();
        }

        // Next token is "function"
        n->extendLocation(ctx->get());
        ctx->consume();

        if (!ctx->match(TokenType::Identifier)) {
            ctx->logError("Expected identifier for function name");
            n->m_isError = true;
            return n;
        }

        n->name = ctx->get()->toString();
        n->extendLocation(ctx->get());
        ctx->consume();

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) {
            ctx->logError("Expected '('");
            n->m_isError = true;
            return n;
        }

        n->extendLocation(ctx->get());
        ctx->consume();

        TypedAssignableNode* param = TypedAssignableNode::TryParse(ctx);
        while (param) {
            if (param->isError()) {
                param->destroy();
                ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseParen);
                break;
            }

            n->parameters.push(param);
            n->extendLocation(param);

            if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Comma)) {
                n->extendLocation(ctx->get());
                ctx->consume();
                param = TypedAssignableNode::TryParse(ctx);

                if (!param) {
                    ctx->logError("Expected typed parameter after ','");
                    n->m_isError = true;
                    ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseParen);
                    break;
                }
            } else param = nullptr;
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseParen)) {
            ctx->logError("Expected ')'");
            n->m_isError = true;
            return n;
        }

        n->extendLocation(ctx->get());
        ctx->consume();

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Colon)) {
            ctx->logError("Expected return type specifier");
            n->m_isError = true;
            return n;
        }

        n->extendLocation(ctx->get());
        ctx->consume();

        n->returnType = TypeSpecifierNode::TryParse(ctx);
        if (!n->returnType) {
            ctx->logError("Expected type specifier after ':'");
            n->m_isError = true;
            return n;
        }

        if (n->returnType->isError()) {
            n->returnType->destroy();
            n->returnType = nullptr;
            ctx->skipToAnyMatched({
                Match(TokenType::EndOfStatement),
                Match(TokenType::Symbol, TokenSubType::Symbol_OpenBrace)
            });
        } else {
            n->extendLocation(n->returnType);
        }

        if (ctx->match(TokenType::EndOfStatement)) {
            ctx->consume();
            return n;
        }

        n->body = StatementBlockNode::TryParse(ctx);
        if (!n->body) {
            ctx->logError("Expected function body or ';'");
            n->m_isError = true;
            return n;
        }

        n->extendLocation(n->body);
        return n;
    }
};
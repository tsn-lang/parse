#include <parse/classes/ClassPropertyGetterNode.h>
#include <parse/Context.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <tokenize/Token.h>
#include <utils/Array.hpp>

namespace parse {
    ClassPropertyGetterNode::ClassPropertyGetterNode(Context* ctx)
        : Node(ctx, NodeType::ClassPropertyGetterNode), isAsync(false), isStatic(false), isPublic(true), returnType(nullptr), body(nullptr)
    {}
    ClassPropertyGetterNode::~ClassPropertyGetterNode() {}
    void ClassPropertyGetterNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ClassPropertyGetterNode* ClassPropertyGetterNode::Create(Context* ctx) { return new (ctx->allocNode()) ClassPropertyGetterNode(ctx); }

    ClassPropertyGetterNode* ClassPropertyGetterNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        ctx->begin();

        ClassPropertyGetterNode* n = Create(ctx);

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Private)) {
            ctx->consume(n);

            n->isPublic = false;
        } else if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Public)) {
            ctx->consume(n);

            n->isPublic = true;
        }

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Static)) {
            ctx->consume(n);

            n->isStatic = true;
        }

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Async)) {
            ctx->consume(n);

            n->isAsync = true;
        }
        
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Get)) {
            n->destroy();
            ctx->rollback();
            return nullptr;
        }

        // Safe to commit now, getter declarations are the only rule that match the pattern:
        // ["public" | "private"], ["static"], ["async"], ["get"]
        ctx->commit();

        ctx->consume(n);

        if (!ctx->match(TokenType::Identifier)) {
            ctx->logError("Expected identifier");
            n->m_isError = true;
            if (!ctx->skipToAnyMatched({
                // open parameter list
                Match(TokenType::Symbol, TokenSubType::Symbol_OpenParen),
            })) return n;
        } else {
            const Token* nameTok = ctx->get();
            n->extendLocation(nameTok);
            n->propertyName = nameTok->toString();
            ctx->consume();
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) {
            ctx->logError("Expected '('");
            n->m_isError = true;
            if (!ctx->skipToAnyMatched({
                // parameter name
                Match(TokenType::Identifier),

                // end parameter list
                Match(TokenType::Symbol, TokenSubType::Symbol_CloseParen)
            })) return n;
        } else {
            ctx->consume(n);
        }

        TypedAssignableNode* param = TypedAssignableNode::TryParse(ctx);
        while (param) {
            if (param->isError()) {
                param->destroy();
                if (!ctx->skipToAnyMatched({
                    Match(TokenType::Symbol, TokenSubType::Symbol_Comma),
                    Match(TokenType::Symbol, TokenSubType::Symbol_CloseParen)
                })) return n;
                break;
            } else {
                n->parameters.push(param);
                n->extendLocation(param);
            }

            if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Comma)) {
                ctx->consume(n);
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

        ctx->consume(n);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Colon)) {
            ctx->logError("Expected return type specifier");
            n->m_isError = true;
            return n;
        }

        ctx->consume(n);

        n->returnType = TypeSpecifierNode::TryParse(ctx);
        if (!n->returnType) {
            ctx->logError("Expected type specifier after ':'");
            n->m_isError = true;
            return n;
        }

        if (n->returnType->isError()) {
            n->returnType->destroy();
            n->returnType = nullptr;
            return n;
        }
        
        n->extendLocation(n->returnType);

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
        
        if (n->isError()) {
            n->body->destroy();
            n->body = nullptr;
            return n;
        }

        n->extendLocation(n->body);
        return n;
    }
};
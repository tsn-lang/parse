#include <parse/classes/ClassPropertySetterNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/misc/TypeParameterListNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>
#include <utils/Array.hpp>

namespace parse {
    ClassPropertySetterNode::ClassPropertySetterNode(Context* ctx)
        : Node(ctx), isAsync(false), isStatic(false), isPublic(true), returnType(nullptr), body(nullptr),
        typeParameters(nullptr)
    {}
    ClassPropertySetterNode::~ClassPropertySetterNode() {}
    void ClassPropertySetterNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ClassPropertySetterNode* ClassPropertySetterNode::Create(Context* ctx) { return new (ctx->allocNode()) ClassPropertySetterNode(ctx); }

    ClassPropertySetterNode* ClassPropertySetterNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        ctx->begin();

        ClassPropertySetterNode* n = Create(ctx);

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
        
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Set)) {
            n->destroy();
            ctx->rollback();
            return nullptr;
        }

        // Safe to commit now, setter declarations are the only rule that match the pattern:
        // ["public" | "private"], ["static"], ["async"], ["set"]
        ctx->commit();

        ctx->consume(n);

        if (!ctx->match(TokenType::Identifier)) {
            ctx->logError("Expected identifier");
            n->m_isError = true;
            if (!ctx->skipToAnyMatched({
                // open parameter list
                Match(TokenType::Symbol, TokenSubType::Symbol_OpenParen),

                // open type parameter list
                Match(TokenType::Symbol, TokenSubType::Operator_LessThan)
            })) return n;
        } else {
            const Token* nameTok = ctx->get();
            n->extendLocation(nameTok);
            n->propertyName = nameTok->toString();
            ctx->consume();
        }

        n->typeParameters = TypeParameterListNode::TryParse(ctx);
        if (n->typeParameters && n->typeParameters->isError()) {
            n->m_isError = true;

            if (!ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) return n;
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
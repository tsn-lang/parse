#include <parse/classes/ClassOperatorMethodNode.h>
#include <parse/Context.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/misc/TypeParameterListNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <tokenize/Token.h>
#include <utils/Array.hpp>

namespace parse {
    ClassOperatorMethodNode::ClassOperatorMethodNode(Context* ctx)
        : Node(ctx), isAsync(false), isPublic(true), isStatic(false), operation(OperatorType::Invalid),
        returnType(nullptr), body(nullptr), typeParameters(nullptr)
    {}
    ClassOperatorMethodNode::~ClassOperatorMethodNode() {}
    void ClassOperatorMethodNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ClassOperatorMethodNode* ClassOperatorMethodNode::Create(Context* ctx) { return new (ctx->allocNode()) ClassOperatorMethodNode(ctx); }

    ClassOperatorMethodNode* ClassOperatorMethodNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        ctx->begin();

        ClassOperatorMethodNode* n = Create(ctx);

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
        
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Operator)) {
            n->destroy();
            ctx->rollback();
            return nullptr;
        }

        // Safe to commit now, operator declarations are the only rule that match the pattern:
        // ["public" | "private"], ["static"], ["async"], ["operator"]
        ctx->commit();

        ctx->consume(n);

        bool allowOptionalPostfixSpecifier = false;

        if (!ctx->match(TokenType::Symbol)) {
            if (ctx->matchAll({
                Match(TokenType::Symbol, TokenSubType::Symbol_OpenParen),
                Match(TokenType::Symbol, TokenSubType::Symbol_CloseParen),
                Match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)
            })) {
                // operator ()(...)
                ctx->consume(n);

                ctx->consume(n);

                n->operation = OperatorType::Call;
            } else if(ctx->matchAll({
                Match(TokenType::Symbol, TokenSubType::Symbol_OpenBracket),
                Match(TokenType::Symbol, TokenSubType::Symbol_CloseBracket),
            })) {
                // operator [](...)
                ctx->consume(n);
                
                ctx->consume(n);

                n->operation = OperatorType::Index;
            } else if(ctx->match(TokenType::Keyword, TokenSubType::Keyword_As)) {
                // operator as(...)
                ctx->consume(n);

                n->operation = OperatorType::Cast;
            } else {
                ctx->logError("Expected valid operator after 'operator'");
                n->m_isError = true;
                if (!ctx->skipToAnyMatched({
                    // open parameter list
                    Match(TokenType::Symbol, TokenSubType::Symbol_OpenParen),
                })) return n;
            }
        } else {
            const Token* operatorTok = ctx->get();
            switch (TokenSubType(operatorTok->subType)) {
                case TokenSubType::Operator_Add:
                case TokenSubType::Operator_Sub:
                case TokenSubType::Operator_Mul:
                case TokenSubType::Operator_Div:
                case TokenSubType::Operator_Mod:
                case TokenSubType::Operator_Exp:
                case TokenSubType::Operator_AddEq:
                case TokenSubType::Operator_SubEq:
                case TokenSubType::Operator_MulEq:
                case TokenSubType::Operator_DivEq:
                case TokenSubType::Operator_ModEq:
                case TokenSubType::Operator_ExpEq:
                case TokenSubType::Operator_BitwiseAnd:
                case TokenSubType::Operator_BitwiseOr:
                case TokenSubType::Operator_BitwiseXOr:
                case TokenSubType::Operator_BitwiseInvert:
                case TokenSubType::Operator_ShiftLeft:
                case TokenSubType::Operator_ShiftRight:
                case TokenSubType::Operator_LogicalAnd:
                case TokenSubType::Operator_LogicalOr:
                case TokenSubType::Operator_BitwiseAndEq:
                case TokenSubType::Operator_BitwiseOrEq:
                case TokenSubType::Operator_BitwiseXOrEq:
                case TokenSubType::Operator_ShiftLeftEq:
                case TokenSubType::Operator_ShiftRightEq:
                case TokenSubType::Operator_LogicalAndEq:
                case TokenSubType::Operator_LogicalOrEq:
                case TokenSubType::Operator_Not:
                case TokenSubType::Operator_Equality:
                case TokenSubType::Operator_Inequality:
                case TokenSubType::Operator_LessThan:
                case TokenSubType::Operator_LessThanEq:
                case TokenSubType::Operator_GreaterThan:
                case TokenSubType::Operator_GreaterThanEq: {
                    n->operation = OperatorType(operatorTok->subType);
                    n->extendLocation(operatorTok);
                    ctx->consume();
                    break;
                }
                case TokenSubType::Operator_Increment:{
                    n->operation = OperatorType::PreIncrement;
                    n->extendLocation(operatorTok);
                    ctx->consume();
                    allowOptionalPostfixSpecifier = true;
                    break;
                }
                case TokenSubType::Operator_Decrement: {
                    n->operation = OperatorType::PreDecrement;
                    n->extendLocation(operatorTok);
                    ctx->consume();
                    allowOptionalPostfixSpecifier = true;
                    break;
                }
                default: {
                    ctx->logError("Expected valid operator after 'operator'");
                    n->m_isError = true;
                    if (!ctx->skipToAnyMatched({
                        // open parameter list
                        Match(TokenType::Symbol, TokenSubType::Symbol_OpenParen),
                    })) return n;
                }
            }
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

        if (allowOptionalPostfixSpecifier) {
            if (ctx->match(TokenType::Identifier, "postfix")) {
                ctx->consume(n);
                if (n->operation == OperatorType::PreIncrement) n->operation = OperatorType::PostIncrement;
                else if (n->operation == OperatorType::PreDecrement) n->operation = OperatorType::PostDecrement;
            }
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

        if (n->parameters.size() == 0) {
            if (n->operation == OperatorType::Sub) n->operation = OperatorType::Negate;
            else if (n->operation == OperatorType::Mul) n->operation = OperatorType::Dereference;
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
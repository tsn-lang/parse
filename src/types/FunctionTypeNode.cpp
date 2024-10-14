#include <parse/types/FunctionTypeNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/Context.h>

namespace parse {
    FunctionTypeNode::FunctionTypeNode(Context* ctx) : Node(ctx), returnType(nullptr) {}
    FunctionTypeNode::~FunctionTypeNode() {}
    void FunctionTypeNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    FunctionTypeNode* FunctionTypeNode::Create(Context* ctx) { return new (ctx->allocNode()) FunctionTypeNode(ctx); }

    FunctionTypeNode* FunctionTypeNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) return nullptr;

        ctx->begin();
        FunctionTypeNode* n = Create(ctx);
        ctx->consume(n);

        TypedAssignableNode* param = TypedAssignableNode::TryParse(ctx);
        if (!param && !ctx->match(TokenType::Symbol, TokenSubType::Symbol_CloseParen)) {
            ctx->rollback();
            n->destroy();
            return nullptr;
        }

        ctx->commit();

        while (param) {
            if (param->isError()) {
                param->destroy();
                ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_CloseParen);
                break;
            }

            n->parameters.push(param);
            n->extendLocation(param);

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

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Arrow)) {
            ctx->logError("Expected '=>'");
            n->m_isError = true;
            return n;
        }

        ctx->consume(n);

        n->returnType = TypeSpecifierNode::TryParse(ctx);
        if (!n->returnType) {
            ctx->logError("Expected return type specifier");
            n->m_isError = true;
        }

        return n;
    }
};
#include <parse/misc/ParameterListNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/Context.h>

namespace parse {
    ParameterListNode::ParameterListNode(Context* ctx) : Node(ctx) {}
    ParameterListNode::~ParameterListNode() {}
    void ParameterListNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ParameterListNode* ParameterListNode::Create(Context* ctx) { return new (ctx->allocNode()) ParameterListNode(ctx); }

    ParameterListNode* ParameterListNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) return nullptr;

        ParameterListNode* n = Create(ctx);
        ctx->consume(n);

        TypedAssignableNode* param = TypedAssignableNode::TryParse(ctx);
        while (param) {
            if (param->isError()) {
                param->destroy();
                ctx->skipTo(TokenType::Symbol, TokenSubType::Symbol_OpenParen);
                break;
            }

            n->parameters.push(param);
            n->extendLocation(param);

            if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Comma)) {
                ctx->consume(n);
                param = TypedAssignableNode::TryParse(ctx);

                if (!param) {
                    ctx->logError("Expected type parameter after ','");
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
        
        return n;
    }
};
#include <parse/misc/TypeParameterListNode.h>
#include <parse/misc/TypeParameterNode.h>
#include <parse/Context.h>

namespace parse {
    TypeParameterListNode::TypeParameterListNode(Context* ctx) : Node(ctx, NodeType::TypeParameterListNode) {}
    TypeParameterListNode::~TypeParameterListNode() {}
    void TypeParameterListNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    TypeParameterListNode* TypeParameterListNode::Create(Context* ctx) { return new (ctx->allocNode()) TypeParameterListNode(ctx); }

    TypeParameterListNode* TypeParameterListNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Symbol, TokenSubType::Operator_LessThan)) return nullptr;

        TypeParameterListNode* n = Create(ctx);
        ctx->consume(n);

        TypeParameterNode* arg = TypeParameterNode::TryParse(ctx);
        while (arg) {
            if (arg->isError()) {
                arg->destroy();
                ctx->skipTo(TokenType::Symbol, TokenSubType::Operator_GreaterThan);
                break;
            }

            n->parameters.push(arg);
            n->extendLocation(arg);

            if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Comma)) {
                ctx->consume(n);
                arg = TypeParameterNode::TryParse(ctx);

                if (!arg) {
                    ctx->logError("Expected type parameter after ','");
                    n->m_isError = true;
                    ctx->skipTo(TokenType::Symbol, TokenSubType::Operator_GreaterThan);
                    break;
                }
            } else arg = nullptr;
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Operator_GreaterThan)) {
            ctx->logError("Expected '>'");
            n->m_isError = true;
            return n;
        }

        ctx->consume(n);
        
        return n;
    }
};
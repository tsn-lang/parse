#include <parse/types/FunctionTypeNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/misc/ParameterListNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/Context.h>

namespace parse {
    FunctionTypeNode::FunctionTypeNode(Context* ctx) : Node(ctx, NodeType::FunctionTypeNode), parameters(nullptr), returnType(nullptr) {}
    void FunctionTypeNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    FunctionTypeNode* FunctionTypeNode::Create(Context* ctx) { return new (ctx->allocNode()) FunctionTypeNode(ctx); }

    FunctionTypeNode* FunctionTypeNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_OpenParen)) return nullptr;

        ctx->begin();
        FunctionTypeNode* n = Create(ctx);

        n->parameters = ParameterListNode::TryParse(ctx);
        if (!n->parameters) {
            ctx->rollback();
            n->destroy();
            return nullptr;
        }

        ctx->commit();

        n->m_isError = n->parameters->isError();
        n->extendLocation(n->parameters);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Arrow)) {
            ctx->logError("Expected '=>'");
            n->m_isError = true;
            return n;
        }

        ctx->consume(n);

        n->returnType = TypeSpecifierNode::TryParse(ctx);
        if (n->returnType) n->extendLocation(n->returnType);
        else {
            ctx->logError("Expected return type specifier");
            n->m_isError = true;
        }

        return n;
    }
};
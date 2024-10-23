#include <parse/literals/ObjectLiteralPropertyNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    ObjectLiteralPropertyNode::ObjectLiteralPropertyNode(Context* ctx) : Node(ctx, NodeType::ObjectLiteralPropertyNode), value(nullptr) {}
    ObjectLiteralPropertyNode::~ObjectLiteralPropertyNode() {}
    void ObjectLiteralPropertyNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ObjectLiteralPropertyNode* ObjectLiteralPropertyNode::Create(Context* ctx) { return new (ctx->allocNode()) ObjectLiteralPropertyNode(ctx); }

    ObjectLiteralPropertyNode* ObjectLiteralPropertyNode::TryParse(Context* ctx) {
        if (!ctx->matchAll({
            Match(TokenType::Identifier),
            Match(TokenType::Symbol, TokenSubType::Symbol_Colon)
        })) return nullptr;

        ObjectLiteralPropertyNode* n = Create(ctx);
        n->name = ctx->get()->toString();
        ctx->consume(n);
        ctx->consume(n);

        n->value = ExpressionNode::TryParse(ctx);

        if (n->value) {
            n->m_isError = n->value->isError();
            n->extendLocation(n->value);
        } else {
            n->m_isError = true;
            ctx->logError("Expected expression");
        }

        return n;
    }
};
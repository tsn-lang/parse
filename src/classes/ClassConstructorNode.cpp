#include <parse/classes/ClassConstructorNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/misc/ParameterListNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <parse/Context.h>

namespace parse {
    ClassConstructorNode::ClassConstructorNode(Context* ctx) : Node(ctx, NodeType::ClassConstructorNode), isPublic(true), body(nullptr), parameters(nullptr) {}
    void ClassConstructorNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ClassConstructorNode* ClassConstructorNode::Create(Context* ctx) { return new (ctx->allocNode()) ClassConstructorNode(ctx); }

    ClassConstructorNode* ClassConstructorNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        ctx->begin();

        ClassConstructorNode* n = Create(ctx);

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Private)) {
            ctx->consume(n);

            n->isPublic = false;
        } else if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Public)) {
            ctx->consume(n);

            n->isPublic = true;
        }
        
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Constructor)) {
            n->destroy();
            ctx->rollback();
            return nullptr;
        }

        ctx->commit();
        ctx->consume(n);

        n->parameters = ParameterListNode::TryParse(ctx);

        if (!n->parameters) {
            ctx->logError("Expected parameter list");
            n->m_isError = true;
            return n;
        }

        n->extendLocation(n->parameters);

        if (ctx->match(TokenType::EndOfStatement)) {
            ctx->consume();
            return n;
        }

        n->body = StatementBlockNode::TryParse(ctx);
        if (!n->body) {
            ctx->logError("Expected function body");
            n->m_isError = true;
            return n;
        }

        n->extendLocation(n->body);
        return n;
    }
};
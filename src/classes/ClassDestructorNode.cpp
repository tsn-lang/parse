#include <parse/classes/ClassDestructorNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/misc/ParameterListNode.h>
#include <parse/Context.h>
#include <utils/Array.hpp>

namespace parse {
    ClassDestructorNode::ClassDestructorNode(Context* ctx) : Node(ctx, NodeType::ClassDestructorNode), isPublic(true), body(nullptr), parameters(nullptr) {}
    void ClassDestructorNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    ClassDestructorNode* ClassDestructorNode::Create(Context* ctx) { return new (ctx->allocNode()) ClassDestructorNode(ctx); }

    ClassDestructorNode* ClassDestructorNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        ctx->begin();

        ClassDestructorNode* n = Create(ctx);

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Private)) {
            ctx->consume(n);

            n->isPublic = false;
        } else if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Public)) {
            ctx->consume(n);

            n->isPublic = true;
        }
        
        if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Destructor)) {
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
#include <parse/types/IdentifierTypeSpecifierNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/misc/IdentifierNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>
#include <utils/Array.hpp>

namespace parse {
    IdentifierTypeSpecifierNode::IdentifierTypeSpecifierNode(Context* ctx) : Node(ctx, NodeType::IdentifierTypeSpecifierNode), name(nullptr) {}
    IdentifierTypeSpecifierNode::~IdentifierTypeSpecifierNode() {}
    void IdentifierTypeSpecifierNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    IdentifierTypeSpecifierNode* IdentifierTypeSpecifierNode::Create(Context* ctx) { return new (ctx->allocNode()) IdentifierTypeSpecifierNode(ctx); }

    IdentifierTypeSpecifierNode* IdentifierTypeSpecifierNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Identifier)) return nullptr;

        ctx->begin();

        IdentifierTypeSpecifierNode* n = Create(ctx);

        n->name = IdentifierNode::TryParse(ctx);
        n->extendLocation(n->name);

        if (!ctx->match(TokenType::Symbol, TokenSubType::Operator_LessThan)) {
            ctx->commit();
            return n;
        }

        ctx->consume(n);

        TypeSpecifierNode* arg = TypeSpecifierNode::TryParse(ctx);
        if (!arg) {
            ctx->rollback();
            n->destroy();
            return nullptr;
        }

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
                arg = TypeSpecifierNode::TryParse(ctx);

                if (!arg) {
                    ctx->logError("Expected type argument after ','");
                    n->m_isError = true;
                    ctx->skipTo(TokenType::Symbol, TokenSubType::Operator_GreaterThan);
                    break;
                }
            } else arg = nullptr;
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Operator_GreaterThan)) {
            if (n->parameters.size() > 1) {
                ctx->logError("Expected '>'");
                n->m_isError = true;
                return n;
            }

            ctx->rollback();
            for (u32 i = 0;i < n->parameters.size();i++) {
                n->parameters[i]->destroy();
            }

            n->destroy();
            return nullptr;
        }
        
        ctx->commit();
        ctx->consume(n);

        return n;
    }
};
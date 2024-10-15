#include <parse/statements/DeclarationStatementNode.h>
#include <parse/misc/IdentifierNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    DeclarationStatementNode::DeclarationStatementNode(Context* ctx)
        : Node(ctx, NodeType::DeclarationStatementNode), isConst(false), isArrayDestructure(false), isObjectDestructure(false), assignable(nullptr),
        typedAssignable(nullptr), initializer(nullptr)
    {}
    DeclarationStatementNode::~DeclarationStatementNode() {}
    void DeclarationStatementNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    DeclarationStatementNode* DeclarationStatementNode::Create(Context* ctx) { return new (ctx->allocNode()) DeclarationStatementNode(ctx); }

    DeclarationStatementNode* DeclarationStatementNode::TryParse(Context* ctx) {
        if (ctx->atEnd()) return nullptr;

        DeclarationStatementNode* n = Create(ctx);

        if (ctx->match(TokenType::Keyword, TokenSubType::Keyword_Const)) {
            n->isConst = true;
        } else if (!ctx->match(TokenType::Keyword, TokenSubType::Keyword_Let)) {
            n->destroy();
            return nullptr;
        }

        ctx->consume(n);

        Node* assignable = n->assignable = IdentifierNode::TryParse(ctx);
        
        if (!assignable) {
            assignable = n->typedAssignable = TypedAssignableNode::TryParse(ctx);
        }

        if (!assignable) {
            if (ctx->matchAny({
                Match(TokenType::Symbol, TokenSubType::Symbol_OpenBrace),
                Match(TokenType::Symbol, TokenSubType::Symbol_OpenBracket),
            })) {
                TokenSubType closeTok = TokenSubType::Symbol_CloseBracket;
                char closeTokStr = ']';
                if (ctx->get()->subType == i32(TokenSubType::Symbol_OpenBrace)) {
                    closeTok = TokenSubType::Symbol_CloseBrace;
                    closeTokStr = '}';
                    n->isObjectDestructure = true;
                } else {
                    n->isArrayDestructure = true;
                }

                ctx->consume(n);

                IdentifierNode* prop = IdentifierNode::TryParse(ctx);
                while (prop) {
                    n->destructureTargets.push(prop);
                    n->extendLocation(prop);

                    if (ctx->match(TokenType::Symbol, TokenSubType::Symbol_Comma)) {
                        ctx->consume(n);

                        prop = IdentifierNode::TryParse(ctx);
                        if (!prop) {
                            ctx->logError("Expected identifier after ','");
                            n->m_isError = true;
                            ctx->skipToAnyMatched({
                                Match(TokenType::Symbol, closeTok),
                                Match(TokenType::Symbol, TokenSubType::Symbol_Equal)
                            });

                            break;
                        }
                    } else prop = nullptr;
                }

                if (!ctx->match(TokenType::Symbol, closeTok)) {
                    ctx->logError("Expected '%c'", closeTokStr);
                    n->m_isError = true;
                    if (!ctx->skipToAnyMatched({
                        Match(TokenType::Symbol, TokenSubType::Symbol_Equal),
                        Match(TokenType::EndOfStatement)
                    })) {
                        return n;
                    }
                }
            }
        } else {
            n->extendLocation(assignable);
        }

        if (!assignable && n->destructureTargets.size() == 0) {
            ctx->logError("Expected assignable, typed assignable, object destructure, or array destructure");
            n->m_isError = true;
            return n;
        }

        if (!ctx->match(TokenType::Symbol, TokenSubType::Symbol_Equal)) return n;

        ctx->consume(n);

        n->initializer = ExpressionNode::TryParse(ctx);
        if (!n->initializer) {
            ctx->logError("Expected expression");
            n->m_isError = true;
            return n;
        }

        return n;
    }
};
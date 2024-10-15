#include <parse/literals/TemplateStringLiteralNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/Context.h>
#include <parse/Tokens.h>
#include <parse/NodeCopy.h>
#include <tokenize/Resource.h>
#include <tokenize/TokenizedSource.h>
#include <tokenize/Token.h>
#include <tokenize/SourceException.h>
#include <utils/Array.hpp>

namespace parse {
    TemplateStringLiteralNode::TemplateStringLiteralNode(Context* ctx) : Node(ctx, NodeType::TemplateStringLiteralNode) {}
    TemplateStringLiteralNode::~TemplateStringLiteralNode() {}
    void TemplateStringLiteralNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    TemplateStringLiteralNode* TemplateStringLiteralNode::Create(Context* ctx) { return new (ctx->allocNode()) TemplateStringLiteralNode(ctx); }

    TemplateStringLiteralNode* TemplateStringLiteralNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Literal, TokenSubType::Literal_TemplateString)) return nullptr;

        TemplateStringLiteralNode* n = Create(ctx);
        const Token* strTok = ctx->get();
        String str = strTok->getContentString();
        u32 strOffset = strTok->contentBeginOffset;

        String part;
        for (u32 i = 0;i < str.size();i++) {
            if (str[i] == '\\') {
                i += parseEscapeSequence(&str[i + 1], part, ctx);
                continue;
            }

            if (str[i] == '$' && str[i + 1] == '{') {
                if (part.size() > 0) {
                    n->segments.push({
                        part,
                        nullptr
                    });

                    part = "";
                }

                // parse expression
                const char* begin = &str[i + 1];
                const char* end = findEndOfTemplateLiteralEmbeddedValue(begin);
                u32 length = u32(end - begin);
                u32 offset = strOffset + i + 1;
                
                SourceLocation exprLoc = ctx->getTokenizedSource()->getSource()->calculateSourceLocationFromRange(
                    offset,
                    offset + length
                );

                String exprSrc;
                exprSrc.copy(begin + 1, length - 2);
                Resource sr(exprSrc.c_str(), n->getLocation().resourceId);
                TokenizedSource* ts = nullptr;

                try {
                    ts = sr.tokenize(ctx->getTokenizedSource()->getTokenSet());
                    Context exprCtx(ts);
                    ExpressionNode* srcExpr = ExpressionNode::TryParse(&exprCtx);

                    ParseState* s = exprCtx.getState();
                    for (u32 m = 0;m < s->messages.size();m++) {
                        ParseMessage& msg = s->messages[m];
                        SourceLocation msgLoc = ctx->getTokenizedSource()->getSource()->calculateSourceLocationFromRange(
                            msg.location.startBufferPosition + offset + 1,
                            msg.location.endBufferPosition + offset + 1
                        );

                        ctx->getState()->messages.push({
                            msg.type,
                            msg.message,
                            msgLoc
                        });
                    }

                    if (!srcExpr) {
                        ctx->getState()->messages.push({
                            ParseMessage::Type::Error,
                            String::Format("Expected expression"),
                            exprLoc
                        });
                    } else {
                        NodeCopy copier(ctx, offset + 1);
                        n->segments.push({
                            String(),
                            copier.copyChildNode(srcExpr)
                        });
                    }
                } catch (const SourceException& exc) {
                    SourceLocation excLoc = ctx->getTokenizedSource()->getSource()->calculateSourceLocationFromRange(
                        exc.getLocation().startBufferPosition + offset + 1,
                        exc.getLocation().endBufferPosition + offset + 1
                    );

                    ctx->getState()->messages.push({
                        ParseMessage::Type::Error,
                        String::Format("Failed to tokenize embedded expression '%s'"),
                        exprLoc
                    });

                    ctx->getState()->messages.push({
                        ParseMessage::Type::Info,
                        exc.message(),
                        excLoc
                    });
                }

                if (ts) delete ts;

                i += length;
                continue;
            }
        
            part += str[i];
        }

        if (part.size() > 0) {
            n->segments.push({
                part,
                nullptr
            });
        }

        ctx->consume(n);
        return n;
    }
};
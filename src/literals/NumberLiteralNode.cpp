#include <parse/literals/NumberLiteralNode.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    NumberLiteralNode::NumberLiteralNode(Context* ctx) : Node(ctx, NodeType::NumberLiteralNode) {
        storageSize = sizeof(i32);
        isSigned = true;
        isFloat = false;
        value.s = 0;
    }
    void NumberLiteralNode::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }
    NumberLiteralNode* NumberLiteralNode::Create(Context* ctx) { return new (ctx->allocNode()) NumberLiteralNode(ctx); }

    NumberLiteralNode* NumberLiteralNode::TryParse(Context* ctx) {
        if (!ctx->match(TokenType::Literal)) return nullptr;
        const Token* tok = ctx->get();

        NumberLiteralNode* n = Create(ctx);
        n->extendLocation(tok);
        
        const char* src = tok->toString().c_str();
        u32 length = tok->location.endBufferPosition - tok->location.startBufferPosition;
        u32 radix = 10;

        if (length > 2) {
            if (src[0] == '0' && (src[1] == 'x' || src[1] == 'X')) {
                radix = 16;
                src += 2;
            } else if (src[0] == '0' && (src[1] == 'b' || src[1] == 'B')) {
                radix = 2;
                src += 2;
            }
        }

        switch (TokenSubType(tok->subType)) {
            case TokenSubType::Literal_Int8: {
                n->storageSize = sizeof(i8);
                n->isSigned = true;
                n->isFloat = false;
                n->value.s = strtol(src, nullptr, radix);
                break;
            }
            case TokenSubType::Literal_Int16: {
                n->storageSize = sizeof(i16);
                n->isSigned = true;
                n->isFloat = false;
                n->value.s = strtol(src, nullptr, radix);
                break;
            }
            case TokenSubType::Literal_Int32: {
                n->storageSize = sizeof(i32);
                n->isSigned = true;
                n->isFloat = false;
                n->value.s = strtol(src, nullptr, radix);
                break;
            }
            case TokenSubType::Literal_Int64: {
                n->storageSize = sizeof(i64);
                n->isSigned = true;
                n->isFloat = false;
                n->value.s = strtoll(src, nullptr, radix);
                break;
            }
            case TokenSubType::Literal_UInt8: {
                n->storageSize = sizeof(u8);
                n->isSigned = false;
                n->isFloat = false;
                n->value.u = strtoull(src, nullptr, radix);
                break;
            }
            case TokenSubType::Literal_UInt16: {
                n->storageSize = sizeof(u16);
                n->isSigned = false;
                n->isFloat = false;
                n->value.u = strtoull(src, nullptr, radix);
                break;
            }
            case TokenSubType::Literal_UInt32: {
                n->storageSize = sizeof(u32);
                n->isSigned = false;
                n->isFloat = false;
                n->value.u = strtoull(src, nullptr, radix);
                break;
            }
            case TokenSubType::Literal_UInt64: {
                n->storageSize = sizeof(u64);
                n->isSigned = false;
                n->isFloat = false;
                n->value.u = strtoull(src, nullptr, radix);
                break;
            }
            case TokenSubType::Literal_Float32: {
                n->storageSize = sizeof(f32);
                n->isSigned = true;
                n->isFloat = true;
                n->value.f = strtof(src, nullptr);
                break;
            }
            case TokenSubType::Literal_Float64: {
                n->storageSize = sizeof(f64);
                n->isSigned = true;
                n->isFloat = true;
                n->value.d = strtod(src, nullptr);
                break;
            }
        }

        ctx->consume();
        return n;
    }
};
#pragma once
#include <parse/misc/Node.h>

namespace parse {
    class Context;

    class NumberLiteralNode : public Node {
        public:
            virtual void acceptVisitor(INodeVisitor* visitor);
            static NumberLiteralNode* Create(Context* ctx);
            static NumberLiteralNode* TryParse(Context* ctx);

            u32 storageSize;
            bool isSigned;
            bool isFloat;
            union {
                i64 s;
                u64 u;
                f32 f;
                f64 d;
            } value;

        private:
            NumberLiteralNode(Context* ctx);
    };
};
#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.h>
#include <utils/String.h>

namespace parse {
    class Context;

    class TemplateStringLiteralNode : public Node {
        public:
            virtual ~TemplateStringLiteralNode();
            virtual void acceptVisitor(INodeVisitor* visitor);
            static TemplateStringLiteralNode* Create(Context* ctx);
            static TemplateStringLiteralNode* TryParse(Context* ctx);

            struct Segment {
                /** If text.size() > 0 then expr will be null */
                String text;

                /** If expr is not null then test.size() will be 0 */
                ExpressionNode* expr;
            };

            Array<Segment> segments;

        private:
            TemplateStringLiteralNode(Context* ctx);
    };
};
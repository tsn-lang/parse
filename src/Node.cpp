#include <parse/Node.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    Node::Node(Context* ctx) : m_ctx(ctx), m_isError(false) {
        const Token* curTok = ctx->get();
        if (curTok) {
            m_location.resourceId = curTok->location.resourceId;
            m_location.startBufferPosition = curTok->location.startBufferPosition;
            m_location.startLine = curTok->location.startLine;
            m_location.startColumn = curTok->location.startColumn;
            m_location.endBufferPosition = m_location.startBufferPosition;
            m_location.endLine = m_location.endLine;
            m_location.endColumn = m_location.endColumn;
        }
    }

    void Node::destroy() {
        m_ctx->freeNode(this);
    }

    Context* Node::getContext() const {
        return m_ctx;
    }

    const SourceLocation& Node::getLocation() const {
        return m_location;
    }
    
    void Node::extendLocation(Node* node) {
        if (!node || node->m_isError) return;

        const SourceLocation& src = node->m_location;

        if (src.startBufferPosition < m_location.startBufferPosition) {
            m_location.startBufferPosition = src.startBufferPosition;
        }

        if (src.startLine < m_location.startLine) {
            m_location.startLine = src.startLine;
            m_location.startColumn = src.startColumn;
        } else if (src.startLine == m_location.startLine && src.startColumn < m_location.startColumn) {
            m_location.startColumn = src.startColumn;
        }

        if (src.endBufferPosition > m_location.endBufferPosition) {
            m_location.endBufferPosition = src.endBufferPosition;
        }

        if (src.endLine > m_location.endLine) {
            m_location.endLine = src.endLine;
            m_location.endColumn = src.endColumn;
        } else if (src.endLine == m_location.endLine && src.endColumn > m_location.endColumn) {
            m_location.endColumn = src.endColumn;
        }
    }
    
    void Node::extendLocation(const Token* tok) {
        if (!tok) return;

        const SourceLocation& src = tok->location;

        if (src.startBufferPosition < m_location.startBufferPosition) {
            m_location.startBufferPosition = src.startBufferPosition;
        }

        if (src.startLine < m_location.startLine) {
            m_location.startLine = src.startLine;
            m_location.startColumn = src.startColumn;
        } else if (src.startLine == m_location.startLine && src.startColumn < m_location.startColumn) {
            m_location.startColumn = src.startColumn;
        }

        if (src.endBufferPosition > m_location.endBufferPosition) {
            m_location.endBufferPosition = src.endBufferPosition;
        }

        if (src.endLine > m_location.endLine) {
            m_location.endLine = src.endLine;
            m_location.endColumn = src.endColumn;
        } else if (src.endLine == m_location.endLine && src.endColumn > m_location.endColumn) {
            m_location.endColumn = src.endColumn;
        }
    }

    bool Node::isError() const {
        return m_isError;
    }

    void Node::acceptVisitor(INodeVisitor* visitor) {
    }
};
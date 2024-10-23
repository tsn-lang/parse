#include <parse/misc/Node.h>
#include <parse/Context.h>
#include <tokenize/Token.h>

namespace parse {
    void INodeVisitor::visit(ArrayLiteralNode* node) {}
    void INodeVisitor::visit(ArrayTypeNode* node) {}
    void INodeVisitor::visit(ArrowFunctionExpressionNode* node) {}
    void INodeVisitor::visit(AwaitExpressionNode* node) {}
    void INodeVisitor::visit(BinaryExpressionNode* node) {}
    void INodeVisitor::visit(BooleanLiteralNode* node) {}
    void INodeVisitor::visit(BreakStatementNode* node) {}
    void INodeVisitor::visit(CallExpressionNode* node) {}
    void INodeVisitor::visit(CastExpressionNode* node) {}
    void INodeVisitor::visit(CatchStatementNode* node) {}
    void INodeVisitor::visit(ClassConstructorNode* node) {}
    void INodeVisitor::visit(ClassDestructorNode* node) {}
    void INodeVisitor::visit(ClassMethodNode* node) {}
    void INodeVisitor::visit(ClassNode* node) {}
    void INodeVisitor::visit(ClassOperatorMethodNode* node) {}
    void INodeVisitor::visit(ClassPropertyGetterNode* node) {}
    void INodeVisitor::visit(ClassPropertyNode* node) {}
    void INodeVisitor::visit(ClassPropertySetterNode* node) {}
    void INodeVisitor::visit(ConditionalExpressionNode* node) {}
    void INodeVisitor::visit(ContinueStatementNode* node) {}
    void INodeVisitor::visit(DeclarationStatementNode* node) {}
    void INodeVisitor::visit(DeleteStatementNode* node) {}
    void INodeVisitor::visit(DoWhileLoopNode* node) {}
    void INodeVisitor::visit(EmptyStatementNode* node) {}
    void INodeVisitor::visit(EnumFieldNode* node) {}
    void INodeVisitor::visit(EnumNode* node) {}
    void INodeVisitor::visit(ExportStatementNode* node) {}
    void INodeVisitor::visit(ExpressionNode* node) {}
    void INodeVisitor::visit(ExpressionSequenceNode* node) {}
    void INodeVisitor::visit(ExpressionStatementNode* node) {}
    void INodeVisitor::visit(ForLoopNode* node) {}
    void INodeVisitor::visit(FunctionExpressionNode* node) {}
    void INodeVisitor::visit(FunctionNode* node) {}
    void INodeVisitor::visit(FunctionTypeNode* node) {}
    void INodeVisitor::visit(IdentifierNode* node) {}
    void INodeVisitor::visit(IdentifierTypeSpecifierNode* node) {}
    void INodeVisitor::visit(IfStatementNode* node) {}
    void INodeVisitor::visit(IndexExpressionNode* node) {}
    void INodeVisitor::visit(ImportAllStatementNode* node) {}
    void INodeVisitor::visit(ImportSelectStatementNode* node) {}
    void INodeVisitor::visit(MemberExpressionNode* node) {}
    void INodeVisitor::visit(ModuleNode* node) {}
    void INodeVisitor::visit(NewExpressionNode* node) {}
    void INodeVisitor::visit(NullLiteralNode* node) {}
    void INodeVisitor::visit(NumberLiteralNode* node) {}
    void INodeVisitor::visit(ObjectLiteralNode* node) {}
    void INodeVisitor::visit(ObjectLiteralPropertyNode* node) {}
    void INodeVisitor::visit(ObjectTypeNode* node) {}
    void INodeVisitor::visit(ParameterListNode* node) {}
    void INodeVisitor::visit(ReturnStatementNode* node) {}
    void INodeVisitor::visit(SizeOfExpressionNode* node) {}
    void INodeVisitor::visit(StatementBlockNode* node) {}
    void INodeVisitor::visit(StatementNode* node) {}
    void INodeVisitor::visit(StringLiteralNode* node) {}
    void INodeVisitor::visit(SwitchCaseNode* node) {}
    void INodeVisitor::visit(SwitchStatementNode* node) {}
    void INodeVisitor::visit(TemplateStringLiteralNode* node) {}
    void INodeVisitor::visit(ThisExpressionNode* node) {}
    void INodeVisitor::visit(ThrowStatementNode* node) {}
    void INodeVisitor::visit(TopLevelNode* node) {}
    void INodeVisitor::visit(TryCatchStatementNode* node) {}
    void INodeVisitor::visit(TypedAssignableNode* node) {}
    void INodeVisitor::visit(TypeInfoExpressionNode* node) {}
    void INodeVisitor::visit(TypeNode* node) {}
    void INodeVisitor::visit(TypeParameterListNode* node) {}
    void INodeVisitor::visit(TypeParameterNode* node) {}
    void INodeVisitor::visit(TypeSpecifierNode* node) {}
    void INodeVisitor::visit(UnaryExpressionNode* node) {}
    void INodeVisitor::visit(WhileLoopNode* node) {}

    Node::Node(Context* ctx, NodeType type) : m_ctx(ctx), m_isError(false), m_type(type) {
        const Token* curTok = ctx->get();
        if (curTok) {
            m_location.resourceId = curTok->location.resourceId;
            m_location.startBufferPosition = curTok->location.startBufferPosition;
            m_location.startLine = curTok->location.startLine;
            m_location.startColumn = curTok->location.startColumn;
            m_location.endBufferPosition = curTok->location.endBufferPosition;
            m_location.endLine = curTok->location.endLine;
            m_location.endColumn = curTok->location.endColumn;
        }
    }

    Node::~Node() {
    }

    void Node::destroy() {
        m_ctx->freeNode(this);
    }

    NodeType Node::getType() const {
        return m_type;
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
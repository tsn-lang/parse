#pragma once
#include <parse/misc/Node.h>
#include <utils/Array.hpp>

namespace parse {
    class NodeCopy : public INodeVisitor {
        public:
            NodeCopy(Context* destContext, u32 bufferOffset);

            void offsetSourceLocation(SourceLocation& loc);
            void useResult(Node* resultNode, Node* sourceNode);

            template <typename NodeType>
            NodeType* copyChildNode(NodeType* in) {
                if (!in) return nullptr;

                in->acceptVisitor(this);
                return (NodeType*)result;
            }

            template <typename NodeType>
            void copyChildNodeArray(const Array<NodeType*> in, Array<NodeType*>& out) {
                for (u32 i = 0;i < in.size();i++) {
                    out.push(copyChildNode(in[i]));
                }
            }
            
            virtual void visit(ArrayTypeNode* node);
            virtual void visit(ArrowFunctionExpressionNode* node);
            virtual void visit(AwaitExpressionNode* node);
            virtual void visit(BinaryExpressionNode* node);
            virtual void visit(BooleanLiteralNode* node);
            virtual void visit(BreakStatementNode* node);
            virtual void visit(CallExpressionNode* node);
            virtual void visit(CastExpressionNode* node);
            virtual void visit(CatchStatementNode* node);
            virtual void visit(ClassConstructorNode* node);
            virtual void visit(ClassDestructorNode* node);
            virtual void visit(ClassMethodNode* node);
            virtual void visit(ClassNode* node);
            virtual void visit(ClassOperatorMethodNode* node);
            virtual void visit(ClassPropertyGetterNode* node);
            virtual void visit(ClassPropertyNode* node);
            virtual void visit(ClassPropertySetterNode* node);
            virtual void visit(ConditionalExpressionNode* node);
            virtual void visit(ContinueStatementNode* node);
            virtual void visit(DeclarationStatementNode* node);
            virtual void visit(DeleteStatementNode* node);
            virtual void visit(DoWhileLoopNode* node);
            virtual void visit(EmptyStatementNode* node);
            virtual void visit(EnumFieldNode* node);
            virtual void visit(EnumNode* node);
            virtual void visit(ExportStatementNode* node);
            virtual void visit(ExpressionNode* node);
            virtual void visit(ExpressionSequenceNode* node);
            virtual void visit(ExpressionStatementNode* node);
            virtual void visit(ForLoopNode* node);
            virtual void visit(FunctionExpressionNode* node);
            virtual void visit(FunctionNode* node);
            virtual void visit(FunctionTypeNode* node);
            virtual void visit(IdentifierNode* node);
            virtual void visit(IdentifierTypeSpecifierNode* node);
            virtual void visit(IfStatementNode* node);
            virtual void visit(IndexExpressionNode* node);
            virtual void visit(ImportAllStatementNode* node);
            virtual void visit(ImportSelectStatementNode* node);
            virtual void visit(MemberExpressionNode* node);
            virtual void visit(ModuleNode* node);
            virtual void visit(NewExpressionNode* node);
            virtual void visit(NullLiteralNode* node);
            virtual void visit(NumberLiteralNode* node);
            virtual void visit(ObjectTypeNode* node);
            virtual void visit(ReturnStatementNode* node);
            virtual void visit(SizeOfExpressionNode* node);
            virtual void visit(StatementBlockNode* node);
            virtual void visit(StatementNode* node);
            virtual void visit(StringLiteralNode* node);
            virtual void visit(SwitchCaseNode* node);
            virtual void visit(SwitchStatementNode* node);
            virtual void visit(TemplateStringLiteralNode* node);
            virtual void visit(ThisExpressionNode* node);
            virtual void visit(ThrowStatementNode* node);
            virtual void visit(TopLevelNode* node);
            virtual void visit(TryCatchStatementNode* node);
            virtual void visit(TypedAssignableNode* node);
            virtual void visit(TypeInfoExpressionNode* node);
            virtual void visit(TypeNode* node);
            virtual void visit(TypeParameterListNode* node);
            virtual void visit(TypeParameterNode* node);
            virtual void visit(TypeSpecifierNode* node);
            virtual void visit(UnaryExpressionNode* node);
            virtual void visit(WhileLoopNode* node);

            Node* result;

        protected:
            Context* m_ctx;
            u32 m_offset;
    };
};
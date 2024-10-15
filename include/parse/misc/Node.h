#pragma once
#include <parse/types.h>
#include <tokenize/SourceLocation.h>

namespace tokenize {
    struct Token;
};

namespace parse {
    class Context;
    class ArrayTypeNode;
    class ArrowFunctionExpressionNode;
    class AwaitExpressionNode;
    class BinaryExpressionNode;
    class BooleanLiteralNode;
    class BreakStatementNode;
    class CallExpressionNode;
    class CastExpressionNode;
    class CatchStatementNode;
    class ClassConstructorNode;
    class ClassDestructorNode;
    class ClassMethodNode;
    class ClassNode;
    class ClassOperatorMethodNode;
    class ClassPropertyGetterNode;
    class ClassPropertyNode;
    class ClassPropertySetterNode;
    class ConditionalExpressionNode;
    class ContinueStatementNode;
    class DeclarationStatementNode;
    class DeleteStatementNode;
    class DoWhileLoopNode;
    class EmptyStatementNode;
    class EnumFieldNode;
    class EnumNode;
    class ExportStatementNode;
    class ExpressionNode;
    class ExpressionSequenceNode;
    class ExpressionStatementNode;
    class ForLoopNode;
    class FunctionExpressionNode;
    class FunctionNode;
    class FunctionTypeNode;
    class IdentifierNode;
    class IdentifierTypeSpecifierNode;
    class IfStatementNode;
    class ImportAllStatementNode;
    class ImportSelectStatementNode;
    class IndexExpressionNode;
    class MemberExpressionNode;
    class ModuleNode;
    class NewExpressionNode;
    class NullLiteralNode;
    class NumberLiteralNode;
    class ObjectTypeNode;
    class ParameterListNode;
    class ReturnStatementNode;
    class SizeOfExpressionNode;
    class StatementBlockNode;
    class StatementNode;
    class StringLiteralNode;
    class SwitchCaseNode;
    class SwitchStatementNode;
    class TemplateStringLiteralNode;
    class ThisExpressionNode;
    class ThrowStatementNode;
    class TopLevelNode;
    class TryCatchStatementNode;
    class TypedAssignableNode;
    class TypeInfoExpressionNode;
    class TypeNode;
    class TypeParameterListNode;
    class TypeParameterNode;
    class TypeSpecifierNode;
    class UnaryExpressionNode;
    class WhileLoopNode;

    class INodeVisitor {
        public:
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
            virtual void visit(ParameterListNode* node);
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
    };

    class Node {
        public:
            Node(Context* ctx, NodeType type);
            virtual ~Node();

            void destroy();

            NodeType getType() const;
            Context* getContext() const;
            const SourceLocation& getLocation() const;
            void extendLocation(Node* node);
            void extendLocation(const Token* tok);
            bool isError() const;

            virtual void acceptVisitor(INodeVisitor* visitor);
        
        protected:
            friend class NodeCopy;

            Context* m_ctx;
            NodeType m_type;
            SourceLocation m_location;
            bool m_isError;
    };
};
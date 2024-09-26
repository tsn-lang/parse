#pragma once
#include <parse/types.h>
#include <tokenize/SourceLocation.h>

namespace tokenize {
    struct Token;
};

namespace parse {
    class Context;
    class ErrorNode;
    class ModuleNode;
    class TopLevelNode;
    class StatementNode;
    class FunctionNode;
    class ClassNode;
    class ClassPropertyNode;
    class ClassMethodNode;
    class ClassPropertyGetterNode;
    class ClassPropertySetterNode;
    class ClassOperatorMethodNode;
    class ClassConstructorNode;
    class ClassDestructorNode;
    class TypeNode;
    class EnumNode;
    class IdentifierNode;
    class NumberLiteralNode;
    class BooleanLiteralNode;
    class StringLiteralNode;
    class ExpressionNode;
    class TypedAssignableNode;
    class StatementBlockNode;
    class AliasTypeNode;
    class FunctionTypeNode;
    class ObjectTypeNode;
    class ObjectTypePropertyNode;
    class ArrayTypeNode;
    class ArrayTypeElementNode;
    class TypeSpecifierNode;

    class INodeVisitor {
        public:
            virtual void visit(ErrorNode* node);
            virtual void visit(ModuleNode* node);
            virtual void visit(TopLevelNode* node);
            virtual void visit(StatementNode* node);
            virtual void visit(FunctionNode* node);
            virtual void visit(ClassNode* node);
            virtual void visit(ClassPropertyNode* node);
            virtual void visit(ClassMethodNode* node);
            virtual void visit(ClassPropertyGetterNode* node);
            virtual void visit(ClassPropertySetterNode* node);
            virtual void visit(ClassOperatorMethodNode* node);
            virtual void visit(ClassConstructorNode* node);
            virtual void visit(ClassDestructorNode* node);
            virtual void visit(TypeNode* node);
            virtual void visit(EnumNode* node);
            virtual void visit(IdentifierNode* node);
            virtual void visit(NumberLiteralNode* node);
            virtual void visit(BooleanLiteralNode* node);
            virtual void visit(StringLiteralNode* node);
            virtual void visit(ExpressionNode* node);
            virtual void visit(TypedAssignableNode* node);
            virtual void visit(StatementBlockNode* node);
            virtual void visit(AliasTypeNode* node);
            virtual void visit(FunctionTypeNode* node);
            virtual void visit(ObjectTypeNode* node);
            virtual void visit(ObjectTypePropertyNode* node);
            virtual void visit(ArrayTypeNode* node);
            virtual void visit(ArrayTypeElementNode* node);
            virtual void visit(TypeSpecifierNode* node);
    };

    class Node {
        public:
            Node(Context* ctx);

            void destroy();

            Context* getContext() const;
            const SourceLocation& getLocation() const;
            void extendLocation(Node* node);
            void extendLocation(const Token* tok);
            bool isError() const;

            virtual void acceptVisitor(INodeVisitor* visitor);
        
        protected:
            Context* m_ctx;
            SourceLocation m_location;
            bool m_isError;
    };
};
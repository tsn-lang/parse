#include <parse/NodeCopy.h>
#include <parse/Context.h>
#include <tokenize/TokenizedSource.h>
#include <tokenize/Resource.h>

#include <parse/classes/ClassConstructorNode.h>
#include <parse/classes/ClassDestructorNode.h>
#include <parse/classes/ClassMethodNode.h>
#include <parse/classes/ClassOperatorMethodNode.h>
#include <parse/classes/ClassPropertyGetterNode.h>
#include <parse/classes/ClassPropertyNode.h>
#include <parse/classes/ClassPropertySetterNode.h>
#include <parse/declarations/ClassNode.h>
#include <parse/declarations/EnumFieldNode.h>
#include <parse/declarations/EnumNode.h>
#include <parse/declarations/FunctionNode.h>
#include <parse/declarations/TypeNode.h>
#include <parse/expressions/ArrowFunctionExpressionNode.h>
#include <parse/expressions/AwaitExpressionNode.h>
#include <parse/expressions/BinaryExpressionNode.h>
#include <parse/expressions/CallExpressionNode.h>
#include <parse/expressions/CastExpressionNode.h>
#include <parse/expressions/ConditionalExpressionNode.h>
#include <parse/expressions/ExpressionSequenceNode.h>
#include <parse/expressions/FunctionExpressionNode.h>
#include <parse/expressions/IndexExpressionNode.h>
#include <parse/expressions/MemberExpressionNode.h>
#include <parse/expressions/NewExpressionNode.h>
#include <parse/expressions/SizeOfExpressionNode.h>
#include <parse/expressions/ThisExpressionNode.h>
#include <parse/expressions/TypeInfoExpressionNode.h>
#include <parse/expressions/UnaryExpressionNode.h>
#include <parse/literals/BooleanLiteralNode.h>
#include <parse/literals/NullLiteralNode.h>
#include <parse/literals/NumberLiteralNode.h>
#include <parse/literals/StringLiteralNode.h>
#include <parse/literals/TemplateStringLiteralNode.h>
#include <parse/misc/IdentifierNode.h>
#include <parse/misc/ModuleNode.h>
#include <parse/misc/Node.h>
#include <parse/misc/ParameterListNode.h>
#include <parse/misc/TypedAssignableNode.h>
#include <parse/misc/TypeParameterListNode.h>
#include <parse/misc/TypeParameterNode.h>
#include <parse/proxies/ExpressionNode.h>
#include <parse/proxies/StatementNode.h>
#include <parse/proxies/TopLevelNode.h>
#include <parse/statements/BreakStatementNode.h>
#include <parse/statements/CatchStatementNode.h>
#include <parse/statements/ContinueStatementNode.h>
#include <parse/statements/DeclarationStatementNode.h>
#include <parse/statements/DeleteStatementNode.h>
#include <parse/statements/DoWhileLoopNode.h>
#include <parse/statements/EmptyStatementNode.h>
#include <parse/statements/ExportStatementNode.h>
#include <parse/statements/ExpressionStatementNode.h>
#include <parse/statements/ForLoopNode.h>
#include <parse/statements/IfStatementNode.h>
#include <parse/statements/ImportAllStatementNode.h>
#include <parse/statements/ImportSelectStatementNode.h>
#include <parse/statements/ReturnStatementNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <parse/statements/StatementBlockNode.h>
#include <parse/statements/SwitchCaseNode.h>
#include <parse/statements/SwitchStatementNode.h>
#include <parse/statements/ThrowStatementNode.h>
#include <parse/statements/TryCatchStatementNode.h>
#include <parse/statements/WhileLoopNode.h>
#include <parse/types/ArrayTypeNode.h>
#include <parse/types/FunctionTypeNode.h>
#include <parse/types/IdentifierTypeSpecifierNode.h>
#include <parse/types/ObjectTypeNode.h>
#include <parse/types/TypeSpecifierNode.h>

namespace parse {
    NodeCopy::NodeCopy(Context* destContext, u32 bufferOffset) : m_ctx(destContext), m_offset(bufferOffset), result(nullptr) {
    }

    void NodeCopy::offsetSourceLocation(SourceLocation& loc) {
        SourceLocation offsetLoc = m_ctx->getTokenizedSource()->getSource()->calculateSourceLocationFromRange(
            loc.startBufferPosition + m_offset,
            loc.endBufferPosition + m_offset
        );

        memcpy(&loc, &offsetLoc, sizeof(SourceLocation));
    }
    
    void NodeCopy::useResult(Node* resultNode, Node* sourceNode) {
        resultNode->m_location = sourceNode->getLocation();
        resultNode->m_isError = sourceNode->m_isError;
        offsetSourceLocation(resultNode->m_location);
        result = resultNode;
    }

    void NodeCopy::visit(ArrayTypeNode* node) {
        ArrayTypeNode* n = ArrayTypeNode::Create(m_ctx);

        copyChildNodeArray(node->elements, n->elements);

        useResult(n, node);
    }

    void NodeCopy::visit(ArrowFunctionExpressionNode* node) {
        ArrowFunctionExpressionNode* n = ArrowFunctionExpressionNode::Create(m_ctx);

        n->parameters = copyChildNode(node->parameters);
        n->returnType = copyChildNode(node->returnType);
        n->body = copyChildNode(node->body);

        useResult(n, node);
    }
    
    void NodeCopy::visit(AwaitExpressionNode* node) {
        AwaitExpressionNode* n = AwaitExpressionNode::Create(m_ctx);

        n->expr = copyChildNode(node->expr);

        useResult(n, node);
    }
    
    void NodeCopy::visit(BinaryExpressionNode* node) {
        BinaryExpressionNode* n = BinaryExpressionNode::Create(m_ctx);

        n->lhs = copyChildNode(node->lhs);
        n->rhs = copyChildNode(node->rhs);
        n->operation = node->operation;

        useResult(n, node);
    }

    void NodeCopy::visit(BooleanLiteralNode* node) {
        BooleanLiteralNode* n = BooleanLiteralNode::Create(m_ctx);

        n->value = node->value;

        useResult(n, node);
    }

    void NodeCopy::visit(BreakStatementNode* node) {
        BreakStatementNode* n = BreakStatementNode::Create(m_ctx);
        useResult(n, node);
    }

    void NodeCopy::visit(CallExpressionNode* node) {
        CallExpressionNode* n = CallExpressionNode::Create(m_ctx);

        n->callee = copyChildNode(node->callee);
        n->arguments = copyChildNode(node->arguments);

        useResult(n, node);
    }

    void NodeCopy::visit(CastExpressionNode* node) {
        CastExpressionNode* n = CastExpressionNode::Create(m_ctx);

        n->expr = copyChildNode(node->expr);
        n->asType = copyChildNode(node->asType);

        useResult(n, node);
    }

    void NodeCopy::visit(CatchStatementNode* node) {
        CatchStatementNode* n = CatchStatementNode::Create(m_ctx);

        n->exception = copyChildNode(node->exception);
        n->body = copyChildNode(node->body);

        useResult(n, node);
    }

    void NodeCopy::visit(ClassConstructorNode* node) {
        ClassConstructorNode* n = ClassConstructorNode::Create(m_ctx);

        n->isPublic = node->isPublic;
        n->body = copyChildNode(node->body);
        n->parameters = copyChildNode(node->parameters);

        useResult(n, node);
    }

    void NodeCopy::visit(ClassDestructorNode* node) {
        ClassDestructorNode* n = ClassDestructorNode::Create(m_ctx);

        n->isPublic = node->isPublic;
        n->body = copyChildNode(node->body);
        n->parameters = copyChildNode(node->parameters);

        useResult(n, node);
    }

    void NodeCopy::visit(ClassMethodNode* node) {
        ClassMethodNode* n = ClassMethodNode::Create(m_ctx);

        n->isPublic = node->isPublic;
        n->isStatic = node->isStatic;
        n->isAsync = node->isAsync;
        n->name = node->name;
        n->returnType = copyChildNode(node->returnType);
        n->body = copyChildNode(node->body);
        n->typeParameters = copyChildNode(node->typeParameters);
        n->parameters = copyChildNode(node->parameters);

        useResult(n, node);
    }

    void NodeCopy::visit(ClassNode* node) {
        ClassNode* n = ClassNode::Create(m_ctx);

        n->name = node->name;
        copyChildNodeArray(node->extends, n->extends);
        copyChildNodeArray(node->properties, n->properties);
        copyChildNodeArray(node->methods, n->methods);
        copyChildNodeArray(node->operators, n->operators);
        copyChildNodeArray(node->getters, n->getters);
        copyChildNodeArray(node->setters, n->setters);
        copyChildNodeArray(node->ctors, n->ctors);
        n->dtor = copyChildNode(node->dtor);
        n->typeParameters = copyChildNode(node->typeParameters);

        useResult(n, node);
    }

    void NodeCopy::visit(ClassOperatorMethodNode* node) {
        ClassOperatorMethodNode* n = ClassOperatorMethodNode::Create(m_ctx);

        n->isPublic = node->isPublic;
        n->isStatic = node->isStatic;
        n->isAsync = node->isAsync;
        n->operation = node->operation;
        n->returnType = copyChildNode(node->returnType);
        n->body = copyChildNode(node->body);
        n->typeParameters = copyChildNode(node->typeParameters);
        copyChildNodeArray(node->parameters, n->parameters);

        useResult(n, node);
    }

    void NodeCopy::visit(ClassPropertyGetterNode* node) {
        ClassPropertyGetterNode* n = ClassPropertyGetterNode::Create(m_ctx);

        n->isPublic = node->isPublic;
        n->isStatic = node->isStatic;
        n->isAsync = node->isAsync;
        n->propertyName = node->propertyName;
        n->returnType = copyChildNode(node->returnType);
        n->body = copyChildNode(node->body);
        n->parameters = copyChildNode(node->parameters);

        useResult(n, node);
    }

    void NodeCopy::visit(ClassPropertyNode* node) {
        ClassPropertyNode* n = ClassPropertyNode::Create(m_ctx);

        n->isPublic = node->isPublic;
        n->isStatic = node->isStatic;
        n->name = node->name;
        n->type = copyChildNode(node->type);

        useResult(n, node);
    }

    void NodeCopy::visit(ClassPropertySetterNode* node) {
        ClassPropertySetterNode* n = ClassPropertySetterNode::Create(m_ctx);

        n->isPublic = node->isPublic;
        n->isStatic = node->isStatic;
        n->isAsync = node->isAsync;
        n->propertyName = node->propertyName;
        n->returnType = copyChildNode(node->returnType);
        n->body = copyChildNode(node->body);
        n->typeParameters = copyChildNode(node->typeParameters);
        n->parameters = copyChildNode(node->parameters);

        useResult(n, node);
    }
    
    void NodeCopy::visit(ConditionalExpressionNode* node) {
        ConditionalExpressionNode* n = ConditionalExpressionNode::Create(m_ctx);

        n->condition = copyChildNode(node->condition);
        n->valueOnTrue = copyChildNode(node->valueOnTrue);
        n->valueOnFalse = copyChildNode(node->valueOnFalse);

        useResult(n, node);
    }

    void NodeCopy::visit(ContinueStatementNode* node) {
        ContinueStatementNode* n = ContinueStatementNode::Create(m_ctx);

        useResult(n, node);
    }

    void NodeCopy::visit(DeclarationStatementNode* node) {
        DeclarationStatementNode* n = DeclarationStatementNode::Create(m_ctx);

        n->isConst = node->isConst;
        n->isArrayDestructure = node->isArrayDestructure;
        n->isObjectDestructure = node->isObjectDestructure;
        n->assignable = copyChildNode(node->assignable);
        copyChildNodeArray(node->destructureTargets, n->destructureTargets);
        n->initializer = copyChildNode(node->initializer);

        useResult(n, node);
    }

    void NodeCopy::visit(DeleteStatementNode* node) {
        DeleteStatementNode* n = DeleteStatementNode::Create(m_ctx);

        n->expr = copyChildNode(node->expr);

        useResult(n, node);
    }

    void NodeCopy::visit(DoWhileLoopNode* node) {
        DoWhileLoopNode* n = DoWhileLoopNode::Create(m_ctx);

        n->body = copyChildNode(node->body);
        n->condition = copyChildNode(node->condition);

        useResult(n, node);
    }

    void NodeCopy::visit(EmptyStatementNode* node) {
        EmptyStatementNode* n = EmptyStatementNode::Create(m_ctx);

        useResult(n, node);
    }

    void NodeCopy::visit(EnumFieldNode* node) {
        EnumFieldNode* n = EnumFieldNode::Create(m_ctx);

        n->name = node->name;
        n->value = copyChildNode(node->value);

        useResult(n, node);
    }

    void NodeCopy::visit(EnumNode* node) {
        EnumNode* n = EnumNode::Create(m_ctx);

        n->name = node->name;
        copyChildNodeArray(node->fields, n->fields);

        useResult(n, node);
    }

    void NodeCopy::visit(ExportStatementNode* node) {
        ExportStatementNode* n = ExportStatementNode::Create(m_ctx);

        n->exportable = copyChildNode(n->exportable);

        useResult(n, node);
    }

    void NodeCopy::visit(ExpressionNode* node) {
        ExpressionNode* n = ExpressionNode::Create(m_ctx);

        n->expr = copyChildNode(node->expr);

        useResult(n, node);
    }

    void NodeCopy::visit(ExpressionSequenceNode* node) {
        ExpressionSequenceNode* n = ExpressionSequenceNode::Create(m_ctx);

        copyChildNodeArray(node->expressions, n->expressions);

        useResult(n, node);
    }

    void NodeCopy::visit(ExpressionStatementNode* node) {
        ExpressionStatementNode* n = ExpressionStatementNode::Create(m_ctx);

        copyChildNodeArray(node->expressions, n->expressions);

        useResult(n, node);
    }

    void NodeCopy::visit(ForLoopNode* node) {
        ForLoopNode* n = ForLoopNode::Create(m_ctx);

        n->body = copyChildNode(node->body);
        n->condition = copyChildNode(node->condition);
        n->initializer = copyChildNode(node->initializer);
        n->modifier = copyChildNode(node->modifier);

        useResult(n, node);
    }

    void NodeCopy::visit(FunctionExpressionNode* node) {
        FunctionExpressionNode* n = FunctionExpressionNode::Create(m_ctx);

        n->parameters = copyChildNode(node->parameters);
        n->returnType = copyChildNode(node->returnType);
        n->body = copyChildNode(node->body);

        useResult(n, node);
    }

    void NodeCopy::visit(FunctionNode* node) {
        FunctionNode* n = FunctionNode::Create(m_ctx);

        n->isAsync = node->isAsync;
        n->name = node->name;
        n->returnType = copyChildNode(node->returnType);
        n->body = copyChildNode(node->body);
        n->typeParameters = copyChildNode(node->typeParameters);
        copyChildNodeArray(node->parameters, n->parameters);

        useResult(n, node);
    }

    void NodeCopy::visit(FunctionTypeNode* node) {
        FunctionTypeNode* n = FunctionTypeNode::Create(m_ctx);

        n->returnType = copyChildNode(node->returnType);
        copyChildNodeArray(node->parameters, n->parameters);

        useResult(n, node);
    }

    void NodeCopy::visit(IdentifierNode* node) {
        IdentifierNode* n = IdentifierNode::Create(m_ctx);
        n->text = node->text;
        useResult(n, node);
    }

    void NodeCopy::visit(IdentifierTypeSpecifierNode* node) {
        IdentifierTypeSpecifierNode* n = IdentifierTypeSpecifierNode::Create(m_ctx);

        n->name = node->name;
        copyChildNodeArray(node->parameters, n->parameters);

        useResult(n, node);
    }

    void NodeCopy::visit(IfStatementNode* node) {
        IfStatementNode* n = IfStatementNode::Create(m_ctx);

        n->condition = copyChildNode(node->condition);
        n->body = copyChildNode(node->body);
        n->elseBody = copyChildNode(node->elseBody);

        useResult(n, node);
    }

    void NodeCopy::visit(IndexExpressionNode* node) {
        IndexExpressionNode* n = IndexExpressionNode::Create(m_ctx);

        n->object = copyChildNode(node->object);
        n->index = copyChildNode(node->index);

        useResult(n, node);
    }

    void NodeCopy::visit(ImportAllStatementNode* node) {
        ImportAllStatementNode* n = ImportAllStatementNode::Create(m_ctx);

        n->alias = node->alias;
        n->moduleId = node->moduleId;

        useResult(n, node);
    }

    void NodeCopy::visit(ImportSelectStatementNode* node) {
        ImportSelectStatementNode* n = ImportSelectStatementNode::Create(m_ctx);

        n->moduleId = node->moduleId;
        copyChildNodeArray(node->importList, n->importList);

        useResult(n, node);
    }

    void NodeCopy::visit(MemberExpressionNode* node) {
        MemberExpressionNode* n = MemberExpressionNode::Create(m_ctx);

        n->object = copyChildNode(node->object);
        n->propertyName = node->propertyName;

        useResult(n, node);
    }

    void NodeCopy::visit(ModuleNode* node) {
        ModuleNode* n = ModuleNode::Create(m_ctx);

        copyChildNodeArray(node->statements, n->statements);

        useResult(n, node);
    }

    void NodeCopy::visit(NewExpressionNode* node) {
        NewExpressionNode* n = NewExpressionNode::Create(m_ctx);

        n->type = copyChildNode(node->type);
        n->arguments = copyChildNode(node->arguments);
        n->destination = copyChildNode(node->destination);

        useResult(n, node);
    }

    void NodeCopy::visit(NullLiteralNode* node) {
        NullLiteralNode* n = NullLiteralNode::Create(m_ctx);
        useResult(n, node);
    }

    void NodeCopy::visit(NumberLiteralNode* node) {
        NumberLiteralNode* n = NumberLiteralNode::Create(m_ctx);

        n->storageSize = node->storageSize;
        n->isSigned = node->isSigned;
        n->isFloat = node->isFloat;
        n->value = node->value;

        useResult(n, node);
    }

    void NodeCopy::visit(ObjectTypeNode* node) {
        ObjectTypeNode* n = ObjectTypeNode::Create(m_ctx);

        copyChildNodeArray(node->properties, n->properties);

        useResult(n, node);
    }

    void NodeCopy::visit(ReturnStatementNode* node) {
        ReturnStatementNode* n = ReturnStatementNode::Create(m_ctx);

        n->returnValue = copyChildNode(node->returnValue);

        useResult(n, node);
    }

    void NodeCopy::visit(SizeOfExpressionNode* node) {
        SizeOfExpressionNode* n = SizeOfExpressionNode::Create(m_ctx);

        n->type = copyChildNode(node->type);

        useResult(n, node);
    }

    void NodeCopy::visit(StatementBlockNode* node) {
        StatementBlockNode* n = StatementBlockNode::Create(m_ctx);

        copyChildNodeArray(node->statements, n->statements);

        useResult(n, node);
    }

    void NodeCopy::visit(StatementNode* node) {
        StatementNode* n = StatementNode::Create(m_ctx);

        n->stmt = copyChildNode(node->stmt);

        useResult(n, node);
    }

    void NodeCopy::visit(StringLiteralNode* node) {
        StringLiteralNode* n = StringLiteralNode::Create(m_ctx);

        n->value = node->value;

        useResult(n, node);
    }

    void NodeCopy::visit(SwitchCaseNode* node) {
        SwitchCaseNode* n = SwitchCaseNode::Create(m_ctx);

        n->value = copyChildNode(node->value);
        n->body = copyChildNode(node->body);

        useResult(n, node);
    }

    void NodeCopy::visit(SwitchStatementNode* node) {
        SwitchStatementNode* n = SwitchStatementNode::Create(m_ctx);

        n->value = copyChildNode(node->value);
        copyChildNodeArray(node->cases, n->cases);

        useResult(n, node);
    }

    void NodeCopy::visit(TemplateStringLiteralNode* node) {
        TemplateStringLiteralNode* n = TemplateStringLiteralNode::Create(m_ctx);

        for (u32 i = 0;i < node->segments.size();i++) {
            n->segments.push({
                node->segments[i].text,
                copyChildNode(node->segments[i].expr)
            });
        }

        useResult(n, node);
    }

    void NodeCopy::visit(ThisExpressionNode* node) {
        ThisExpressionNode* n = ThisExpressionNode::Create(m_ctx);
        useResult(n, node);
    }

    void NodeCopy::visit(ThrowStatementNode* node) {
        ThrowStatementNode* n = ThrowStatementNode::Create(m_ctx);

        n->value = copyChildNode(node->value);

        useResult(n, node);
    }

    void NodeCopy::visit(TopLevelNode* node) {
        TopLevelNode* n = TopLevelNode::Create(m_ctx);

        n->statement = copyChildNode(node->statement);

        useResult(n, node);
    }

    void NodeCopy::visit(TryCatchStatementNode* node) {
        TryCatchStatementNode* n = TryCatchStatementNode::Create(m_ctx);

        n->tryBody = copyChildNode(node->tryBody);
        copyChildNodeArray(node->catchBlocks, n->catchBlocks);

        useResult(n, node);
    }

    void NodeCopy::visit(TypedAssignableNode* node) {
        TypedAssignableNode* n = TypedAssignableNode::Create(m_ctx);

        n->name = node->name;
        n->type = copyChildNode(node->type);

        useResult(n, node);
    }

    void NodeCopy::visit(TypeInfoExpressionNode* node) {
        TypeInfoExpressionNode* n = TypeInfoExpressionNode::Create(m_ctx);

        n->type = copyChildNode(node->type);

        useResult(n, node);
    }

    void NodeCopy::visit(TypeNode* node) {
        TypeNode* n = TypeNode::Create(m_ctx);

        n->name = node->name;
        n->type = copyChildNode(node->type);
        n->parameters = copyChildNode(node->parameters);

        useResult(n, node);
    }

    void NodeCopy::visit(TypeParameterNode* node) {
        TypeParameterNode* n = TypeParameterNode::Create(m_ctx);

        n->name = node->name;
        // todo: constraints

        useResult(n, node);
    }

    void NodeCopy::visit(TypeParameterListNode* node) {
        TypeParameterListNode* n = TypeParameterListNode::Create(m_ctx);

        copyChildNodeArray(node->parameters, n->parameters);

        useResult(n, node);
    }

    void NodeCopy::visit(TypeSpecifierNode* node) {
        TypeSpecifierNode* n = TypeSpecifierNode::Create(m_ctx);

        n->mod = node->mod;
        n->type = copyChildNode(node->type);

        useResult(n, node);
    }

    void NodeCopy::visit(UnaryExpressionNode* node) {
        UnaryExpressionNode* n = UnaryExpressionNode::Create(m_ctx);

        n->expr = copyChildNode(node->expr);
        n->operation = node->operation;

        useResult(n, node);
    }

    void NodeCopy::visit(WhileLoopNode* node) {
        WhileLoopNode* n = WhileLoopNode::Create(m_ctx);

        n->body = copyChildNode(node->body);
        n->condition = copyChildNode(node->condition);

        useResult(n, node);
    }
};
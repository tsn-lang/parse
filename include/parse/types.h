#pragma once
#include <tokenize/types.h>

namespace parse {
    using namespace utils;
    using namespace tokenize;

    enum class NodeType {
        Node = 0,
        ArrayLiteralNode,
        ArrayTypeNode,
        ArrowFunctionExpressionNode,
        AwaitExpressionNode,
        BinaryExpressionNode,
        BooleanLiteralNode,
        BreakStatementNode,
        CallExpressionNode,
        CastExpressionNode,
        CatchStatementNode,
        ClassConstructorNode,
        ClassDestructorNode,
        ClassMethodNode,
        ClassNode,
        ClassOperatorMethodNode,
        ClassPropertyGetterNode,
        ClassPropertyNode,
        ClassPropertySetterNode,
        ConditionalExpressionNode,
        ContinueStatementNode,
        DeclarationStatementNode,
        DeleteStatementNode,
        DoWhileLoopNode,
        EmptyStatementNode,
        EnumFieldNode,
        EnumNode,
        ExportStatementNode,
        ExpressionNode,
        ExpressionSequenceNode,
        ExpressionStatementNode,
        ForLoopNode,
        FunctionExpressionNode,
        FunctionNode,
        FunctionTypeNode,
        IdentifierNode,
        IdentifierTypeSpecifierNode,
        IfStatementNode,
        ImportAllStatementNode,
        ImportSelectStatementNode,
        IndexExpressionNode,
        MemberExpressionNode,
        ModuleNode,
        NewExpressionNode,
        NullLiteralNode,
        NumberLiteralNode,
        ObjectLiteralNode,
        ObjectLiteralPropertyNode,
        ObjectTypeNode,
        ParameterListNode,
        ReturnStatementNode,
        SizeOfExpressionNode,
        StatementBlockNode,
        StatementNode,
        StringLiteralNode,
        SwitchCaseNode,
        SwitchStatementNode,
        TemplateStringLiteralNode,
        ThisExpressionNode,
        ThrowStatementNode,
        TopLevelNode,
        TryCatchStatementNode,
        TypedAssignableNode,
        TypeInfoExpressionNode,
        TypeNode,
        TypeParameterListNode,
        TypeParameterNode,
        TypeSpecifierNode,
        UnaryExpressionNode,
        WhileLoopNode
    };
    
    enum class TokenSubType : i32 {
        None = -1,

        Keyword_As             = 0,
        Keyword_Async          = 1,
        Keyword_Await          = 2,
        Keyword_Break          = 3,
        Keyword_Case           = 4,
        Keyword_Catch          = 5,
        Keyword_Class          = 6,
        Keyword_Const          = 7,
        Keyword_Constructor    = 8,
        Keyword_Continue       = 9,
        Keyword_Default        = 10,
        Keyword_Delete         = 11,
        Keyword_Destructor     = 12,
        Keyword_Do             = 13,
        Keyword_Else           = 14,
        Keyword_Enum           = 15,
        Keyword_Export         = 16,
        Keyword_Extends        = 17,
        Keyword_For            = 18,
        Keyword_From           = 19,
        Keyword_Function       = 20,
        Keyword_Get            = 21,
        Keyword_If             = 22,
        Keyword_Import         = 23,
        Keyword_Let            = 24,
        Keyword_New            = 25,
        Keyword_Operator       = 26,
        Keyword_Private        = 27,
        Keyword_Public         = 28,
        Keyword_Return         = 29,
        Keyword_Set            = 30,
        Keyword_SizeOf         = 31,
        Keyword_Static         = 32,
        Keyword_Switch         = 33,
        Keyword_This           = 34,
        Keyword_Throw          = 35,
        Keyword_Try            = 36,
        Keyword_Type           = 37,
        Keyword_TypeInfo       = 38,
        Keyword_While          = 39,
        Symbol_Arrow           = 40,
        Symbol_Asterisk        = 41,
        Symbol_CloseBrace      = 42,
        Symbol_CloseBracket    = 43,
        Symbol_CloseParen      = 44,
        Symbol_Colon           = 45,
        Symbol_Comma           = 46,
        Symbol_Dot             = 47,
        Symbol_Equal           = 48,
        Symbol_OpenBrace       = 49,
        Symbol_OpenBracket     = 50,
        Symbol_OpenParen       = 51,
        Symbol_QuestionMark    = 52,
        Operator_Add           = 53,
        Operator_Sub           = 54,
        Operator_Mul           = Symbol_Asterisk,
        Operator_Div           = 55,
        Operator_Mod           = 56,
        Operator_Exp           = 57,
        Operator_AddEq         = 58,
        Operator_SubEq         = 59,
        Operator_MulEq         = 60,
        Operator_DivEq         = 61,
        Operator_ModEq         = 62,
        Operator_ExpEq         = 63,
        Operator_BitwiseAnd    = 64,
        Operator_BitwiseOr     = 65,
        Operator_BitwiseXOr    = 66,
        Operator_BitwiseInvert = 67,
        Operator_ShiftLeft     = 68,
        Operator_ShiftRight    = 69,
        Operator_LogicalAnd    = 70,
        Operator_LogicalOr     = 71,
        Operator_BitwiseAndEq  = 72,
        Operator_BitwiseOrEq   = 73,
        Operator_BitwiseXOrEq  = 74,
        Operator_ShiftLeftEq   = 75,
        Operator_ShiftRightEq  = 76,
        Operator_LogicalAndEq  = 77,
        Operator_LogicalOrEq   = 78,
        Operator_Increment     = 79,
        Operator_Decrement     = 80,
        Operator_Not           = 81,
        Operator_Assign        = Symbol_Equal,
        Operator_Equality      = 82,
        Operator_Inequality    = 83,
        Operator_LessThan      = 84,
        Operator_LessThanEq    = 85,
        Operator_GreaterThan   = 86,
        Operator_GreaterThanEq = 87,
        Literal_Int8           = 88,
        Literal_Int16          = 89,
        Literal_Int32          = 90,
        Literal_Int64          = 91,
        Literal_UInt8          = 92,
        Literal_UInt16         = 93,
        Literal_UInt32         = 94,
        Literal_UInt64         = 95,
        Literal_Float32        = 96,
        Literal_Float64        = 97,
        Literal_String         = 98,
        Literal_TemplateString = 99,
        Literal_Null           = 100,
        Literal_True           = 101,
        Literal_False          = 102
    };

    enum class OperatorType : i32 {
        Invalid       = -1,
        Add           = OperatorType(TokenSubType::Operator_Add),
        Sub           = OperatorType(TokenSubType::Operator_Sub),
        Mul           = OperatorType(TokenSubType::Operator_Mul),
        Div           = OperatorType(TokenSubType::Operator_Div),
        Mod           = OperatorType(TokenSubType::Operator_Mod),
        Exp           = OperatorType(TokenSubType::Operator_Exp),
        AddEq         = OperatorType(TokenSubType::Operator_AddEq),
        SubEq         = OperatorType(TokenSubType::Operator_SubEq),
        MulEq         = OperatorType(TokenSubType::Operator_MulEq),
        DivEq         = OperatorType(TokenSubType::Operator_DivEq),
        ModEq         = OperatorType(TokenSubType::Operator_ModEq),
        ExpEq         = OperatorType(TokenSubType::Operator_ExpEq),
        BitwiseAnd    = OperatorType(TokenSubType::Operator_BitwiseAnd),
        BitwiseOr     = OperatorType(TokenSubType::Operator_BitwiseOr),
        BitwiseXOr    = OperatorType(TokenSubType::Operator_BitwiseXOr),
        BitwiseInvert = OperatorType(TokenSubType::Operator_BitwiseInvert),
        ShiftLeft     = OperatorType(TokenSubType::Operator_ShiftLeft),
        ShiftRight    = OperatorType(TokenSubType::Operator_ShiftRight),
        LogicalAnd    = OperatorType(TokenSubType::Operator_LogicalAnd),
        LogicalOr     = OperatorType(TokenSubType::Operator_LogicalOr),
        BitwiseAndEq  = OperatorType(TokenSubType::Operator_BitwiseAndEq),
        BitwiseOrEq   = OperatorType(TokenSubType::Operator_BitwiseOrEq),
        BitwiseXOrEq  = OperatorType(TokenSubType::Operator_BitwiseXOrEq),
        ShiftLeftEq   = OperatorType(TokenSubType::Operator_ShiftLeftEq),
        ShiftRightEq  = OperatorType(TokenSubType::Operator_ShiftRightEq),
        LogicalAndEq  = OperatorType(TokenSubType::Operator_LogicalAndEq),
        LogicalOrEq   = OperatorType(TokenSubType::Operator_LogicalOrEq),
        Not           = OperatorType(TokenSubType::Operator_Not),
        Assign        = OperatorType(TokenSubType::Operator_Assign),
        Equality      = OperatorType(TokenSubType::Operator_Equality),
        Inequality    = OperatorType(TokenSubType::Operator_Inequality),
        LessThan      = OperatorType(TokenSubType::Operator_LessThan),
        LessThanEq    = OperatorType(TokenSubType::Operator_LessThanEq),
        GreaterThan   = OperatorType(TokenSubType::Operator_GreaterThan),
        GreaterThanEq = OperatorType(TokenSubType::Operator_GreaterThanEq),
        Dereference,
        Call,
        Index,
        Cast,
        Negate,
        PreIncrement,
        PostIncrement,
        PreDecrement,
        PostDecrement
    };
};
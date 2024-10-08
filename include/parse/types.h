#pragma once
#include <tokenize/types.h>

namespace parse {
    using namespace utils;
    using namespace tokenize;
    
    enum class TokenSubType : i32 {
        None = -1,

        Keyword_As,
        Keyword_Async,
        Keyword_Case,
        Keyword_Catch,
        Keyword_Class,
        Keyword_Const,
        Keyword_Constructor,
        Keyword_Default,
        Keyword_Delete,
        Keyword_Destructor,
        Keyword_Do,
        Keyword_Else,
        Keyword_Enum,
        Keyword_Export,
        Keyword_Extends,
        Keyword_For,
        Keyword_From,
        Keyword_Function,
        Keyword_Get,
        Keyword_If,
        Keyword_Import,
        Keyword_Let,
        Keyword_New,
        Keyword_Operator,
        Keyword_Private,
        Keyword_Public,
        Keyword_Return,
        Keyword_Set,
        Keyword_SizeOf,
        Keyword_Static,
        Keyword_Switch,
        Keyword_This,
        Keyword_Throw,
        Keyword_Try,
        Keyword_Type,
        Keyword_TypeInfo,
        Keyword_While,

        Symbol_OpenParen,
        Symbol_CloseParen,
        Symbol_OpenBrace,
        Symbol_CloseBrace,
        Symbol_OpenBracket,
        Symbol_CloseBracket,
        Symbol_Comma,
        Symbol_Colon,
        
        Operator_Add,
        Operator_Sub,
        Operator_Mul,
        Operator_Div,
        Operator_Mod,
        Operator_Exp,
        Operator_AddEq,
        Operator_SubEq,
        Operator_MulEq,
        Operator_DivEq,
        Operator_ModEq,
        Operator_ExpEq,
        Operator_BitwiseAnd,
        Operator_BitwiseOr,
        Operator_BitwiseXOr,
        Operator_BitwiseInvert,
        Operator_ShiftLeft,
        Operator_ShiftRight,
        Operator_LogicalAnd,
        Operator_LogicalOr,
        Operator_BitwiseAndEq,
        Operator_BitwiseOrEq,
        Operator_BitwiseXOrEq,
        Operator_ShiftLeftEq,
        Operator_ShiftRightEq,
        Operator_LogicalAndEq,
        Operator_LogicalOrEq,
        Operator_Increment,
        Operator_Decrement,
        Operator_Not,
        Operator_Equality,
        Operator_Inequality,
        Operator_LessThan,
        Operator_LessThanEq,
        Operator_GreaterThan,
        Operator_GreaterThanEq,

        Literal_Int8,
        Literal_Int16,
        Literal_Int32,
        Literal_Int64,
        Literal_UInt8,
        Literal_UInt16,
        Literal_UInt32,
        Literal_UInt64,
        Literal_Float32,
        Literal_Float64,
        Literal_String,
        Literal_TemplateString,
        Literal_Null,
        Literal_True,
        Literal_False
    };

    enum class OperatorType : i32 {
        Invalid       = 0,
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
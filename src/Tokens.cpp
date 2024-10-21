#include <parse/Tokens.h>
#include <parse/Context.h>
#include <utils/String.h>
#include <tokenize/TokenSet.h>

namespace parse {
    const char* findEndOfTemplateLiteralEmbeddedValue(const char* input) {
        char strChar = ' ';
        bool inStr = false;
        i32 braceCount = 0;
        
        while (*input) {
            if (*input == '\'' || *input == '`') {
                strChar = *input;
                inStr = !inStr;
                input++;
                continue;
            }

            if (inStr) {
                if (*input == '\\') {
                    if (*(input + 1) == strChar) {
                        input += 2;
                        continue;
                    }

                    if (strChar == '`' && *(input + 1) == '$') {
                        input += 2;
                        continue;
                    }
                }

                if (strChar == '`' && *input == '$' && *(input + 1) == '{') {
                    input = findEndOfTemplateLiteralEmbeddedValue(input + 1);
                    if (!*input) return input;
                    continue;
                }

                if (*input == strChar) {
                    input++;
                    inStr = false;
                    continue;
                }

                input++;
                continue;
            }

            if (*input == '{') {
                braceCount++;
                input++;
                continue;
            }

            if (*input == '}') {
                braceCount--;
                if (braceCount == 0) break;
                continue;
            }

            input++;
        }

        return input;
    }

    MatchResult matchTemplateString(const char* input, MatchedToken* outMatch) {
        if (*input != '`') return MatchResult::NoMatch;
        
        outMatch->type = TokenType::Literal;
        outMatch->subType = i32(TokenSubType::Literal_TemplateString);
        outMatch->offset = 0;
        outMatch->contentBeginOffset = 1;
        outMatch->contentEndOffset = 1;
        outMatch->length = 1;

        const char* begin = input;
        while (*input) {
            input++;
            if (*input == '\\') {
                // ignore '`' and `$` if they're escaped
                // also make sure '\\`' doesn't escape the '`' (it escapes the '\')

                char nextCh = *(input + 1);
                if (nextCh == '\\' || nextCh == '`' || nextCh == '$') input++;
            } else if (*input == '$' && *(input + 1) == '{') {
                input = findEndOfTemplateLiteralEmbeddedValue(input + 1);
            } else if (*input == '`') {
                input++;
                outMatch->length = input - begin;
                outMatch->contentEndOffset = outMatch->length - 1;
                return MatchResult::Matched;
            }
        }

        outMatch->length = input - begin;
        outMatch->contentEndOffset = outMatch->length;
        return MatchResult::EndNotMatched;
    }

    void setupTokenSet(TokenSet* ts) {
        // negative sign will be parsed separately
        #define FLOAT_REGEX "\\d+(?:(?:(?:\\.\\d*)?(?:[eE][+\\-]?\\d+))|(?:\\.\\d*))"
        #define INT_REGEX "(?:0[xX][0-9a-fA-F]+)|(?:0[bB][01]+)|(?:-?\\d+)"
        #define IDENTIFIER_REGEX "([a-zA-Z_]+)\\w*"
        #define STRING_REGEX "'(?:[^'\\\\]|\\\\.)*'"

        ts->addStringToken("as",              TokenType::Keyword, i32(TokenSubType::Keyword_As            ));
        ts->addStringToken("async",           TokenType::Keyword, i32(TokenSubType::Keyword_Async         ));
        ts->addStringToken("await",           TokenType::Keyword, i32(TokenSubType::Keyword_Await         ));
        ts->addStringToken("case",            TokenType::Keyword, i32(TokenSubType::Keyword_Case          ));
        ts->addStringToken("catch",           TokenType::Keyword, i32(TokenSubType::Keyword_Catch         ));
        ts->addStringToken("class",           TokenType::Keyword, i32(TokenSubType::Keyword_Class         ));
        ts->addStringToken("const",           TokenType::Keyword, i32(TokenSubType::Keyword_Const         ));
        ts->addStringToken("constructor",     TokenType::Keyword, i32(TokenSubType::Keyword_Constructor   ));
        ts->addStringToken("default",         TokenType::Keyword, i32(TokenSubType::Keyword_Default       ));
        ts->addStringToken("delete",          TokenType::Keyword, i32(TokenSubType::Keyword_Delete        ));
        ts->addStringToken("destructor",      TokenType::Keyword, i32(TokenSubType::Keyword_Destructor    ));
        ts->addStringToken("do",              TokenType::Keyword, i32(TokenSubType::Keyword_Do            ));
        ts->addStringToken("else",            TokenType::Keyword, i32(TokenSubType::Keyword_Else          ));
        ts->addStringToken("enum",            TokenType::Keyword, i32(TokenSubType::Keyword_Enum          ));
        ts->addStringToken("export",          TokenType::Keyword, i32(TokenSubType::Keyword_Export        ));
        ts->addStringToken("extends",         TokenType::Keyword, i32(TokenSubType::Keyword_Extends       ));
        ts->addStringToken("for",             TokenType::Keyword, i32(TokenSubType::Keyword_For           ));
        ts->addStringToken("from",            TokenType::Keyword, i32(TokenSubType::Keyword_From          ));
        ts->addStringToken("function",        TokenType::Keyword, i32(TokenSubType::Keyword_Function      ));
        ts->addStringToken("get",             TokenType::Keyword, i32(TokenSubType::Keyword_Get           ));
        ts->addStringToken("if",              TokenType::Keyword, i32(TokenSubType::Keyword_If            ));
        ts->addStringToken("import",          TokenType::Keyword, i32(TokenSubType::Keyword_Import        ));
        ts->addStringToken("let",             TokenType::Keyword, i32(TokenSubType::Keyword_Let           ));
        ts->addStringToken("new",             TokenType::Keyword, i32(TokenSubType::Keyword_New           ));
        ts->addStringToken("operator",        TokenType::Keyword, i32(TokenSubType::Keyword_Operator      ));
        ts->addStringToken("private",         TokenType::Keyword, i32(TokenSubType::Keyword_Private       ));
        ts->addStringToken("public",          TokenType::Keyword, i32(TokenSubType::Keyword_Public        ));
        ts->addStringToken("return",          TokenType::Keyword, i32(TokenSubType::Keyword_Return        ));
        ts->addStringToken("set",             TokenType::Keyword, i32(TokenSubType::Keyword_Set           ));
        ts->addStringToken("sizeof",          TokenType::Keyword, i32(TokenSubType::Keyword_SizeOf        ));
        ts->addStringToken("static",          TokenType::Keyword, i32(TokenSubType::Keyword_Static        ));
        ts->addStringToken("switch",          TokenType::Keyword, i32(TokenSubType::Keyword_Switch        ));
        ts->addStringToken("this",            TokenType::Keyword, i32(TokenSubType::Keyword_This          ));
        ts->addStringToken("throw",           TokenType::Keyword, i32(TokenSubType::Keyword_Throw         ));
        ts->addStringToken("try",             TokenType::Keyword, i32(TokenSubType::Keyword_Try           ));
        ts->addStringToken("type",            TokenType::Keyword, i32(TokenSubType::Keyword_Type          ));
        ts->addStringToken("typeinfo",        TokenType::Keyword, i32(TokenSubType::Keyword_TypeInfo      ));
        ts->addStringToken("while",           TokenType::Keyword, i32(TokenSubType::Keyword_While         ));
        ts->addStringToken("(",               TokenType::Symbol,  i32(TokenSubType::Symbol_OpenParen      ));
        ts->addStringToken(")",               TokenType::Symbol,  i32(TokenSubType::Symbol_CloseParen     ));
        ts->addStringToken("{",               TokenType::Symbol,  i32(TokenSubType::Symbol_OpenBrace      ));
        ts->addStringToken("}",               TokenType::Symbol,  i32(TokenSubType::Symbol_CloseBrace     ));
        ts->addStringToken("[",               TokenType::Symbol,  i32(TokenSubType::Symbol_OpenBracket    ));
        ts->addStringToken("]",               TokenType::Symbol,  i32(TokenSubType::Symbol_CloseBracket   ));
        ts->addStringToken(",",               TokenType::Symbol,  i32(TokenSubType::Symbol_Comma          ));
        ts->addStringToken(":",               TokenType::Symbol,  i32(TokenSubType::Symbol_Colon          ));
        ts->addStringToken(".",               TokenType::Symbol,  i32(TokenSubType::Symbol_Dot            ));
        ts->addStringToken("=>",              TokenType::Symbol,  i32(TokenSubType::Symbol_Arrow          ));
        ts->addStringToken("=",               TokenType::Symbol,  i32(TokenSubType::Symbol_Equal          ));
        ts->addStringToken("+",               TokenType::Symbol,  i32(TokenSubType::Operator_Add          ));
        ts->addStringToken("-",               TokenType::Symbol,  i32(TokenSubType::Operator_Sub          ));
        ts->addStringToken("*",               TokenType::Symbol,  i32(TokenSubType::Operator_Mul          ));
        ts->addStringToken("/",               TokenType::Symbol,  i32(TokenSubType::Operator_Div          ));
        ts->addStringToken("%",               TokenType::Symbol,  i32(TokenSubType::Operator_Mod          ));
        ts->addStringToken("**",              TokenType::Symbol,  i32(TokenSubType::Operator_Exp          ));
        ts->addStringToken("+=",              TokenType::Symbol,  i32(TokenSubType::Operator_AddEq        ));
        ts->addStringToken("-=",              TokenType::Symbol,  i32(TokenSubType::Operator_SubEq        ));
        ts->addStringToken("*=",              TokenType::Symbol,  i32(TokenSubType::Operator_MulEq        ));
        ts->addStringToken("/=",              TokenType::Symbol,  i32(TokenSubType::Operator_DivEq        ));
        ts->addStringToken("%=",              TokenType::Symbol,  i32(TokenSubType::Operator_ModEq        ));
        ts->addStringToken("**=",             TokenType::Symbol,  i32(TokenSubType::Operator_ExpEq        ));
        ts->addStringToken("&",               TokenType::Symbol,  i32(TokenSubType::Operator_BitwiseAnd   ));
        ts->addStringToken("|",               TokenType::Symbol,  i32(TokenSubType::Operator_BitwiseOr    ));
        ts->addStringToken("^",               TokenType::Symbol,  i32(TokenSubType::Operator_BitwiseXOr   ));
        ts->addStringToken("~",               TokenType::Symbol,  i32(TokenSubType::Operator_BitwiseInvert));
        ts->addStringToken("<<",              TokenType::Symbol,  i32(TokenSubType::Operator_ShiftLeft    ));
        ts->addStringToken(">>",              TokenType::Symbol,  i32(TokenSubType::Operator_ShiftRight   ));
        ts->addStringToken("&&",              TokenType::Symbol,  i32(TokenSubType::Operator_LogicalAnd   ));
        ts->addStringToken("||",              TokenType::Symbol,  i32(TokenSubType::Operator_LogicalOr    ));
        ts->addStringToken("&=",              TokenType::Symbol,  i32(TokenSubType::Operator_BitwiseAndEq ));
        ts->addStringToken("|=",              TokenType::Symbol,  i32(TokenSubType::Operator_BitwiseOrEq  ));
        ts->addStringToken("^=",              TokenType::Symbol,  i32(TokenSubType::Operator_BitwiseXOrEq ));
        ts->addStringToken("<<=",             TokenType::Symbol,  i32(TokenSubType::Operator_ShiftLeftEq  ));
        ts->addStringToken(">>=",             TokenType::Symbol,  i32(TokenSubType::Operator_ShiftRightEq ));
        ts->addStringToken("&&=",             TokenType::Symbol,  i32(TokenSubType::Operator_LogicalAndEq ));
        ts->addStringToken("||=",             TokenType::Symbol,  i32(TokenSubType::Operator_LogicalOrEq  ));
        ts->addStringToken("++",              TokenType::Symbol,  i32(TokenSubType::Operator_Increment    ));
        ts->addStringToken("--",              TokenType::Symbol,  i32(TokenSubType::Operator_Decrement    ));
        ts->addStringToken("!",               TokenType::Symbol,  i32(TokenSubType::Operator_Not          ));
        ts->addStringToken("==",              TokenType::Symbol,  i32(TokenSubType::Operator_Equality     ));
        ts->addStringToken("!=",              TokenType::Symbol,  i32(TokenSubType::Operator_Inequality   ));
        ts->addStringToken("<",               TokenType::Symbol,  i32(TokenSubType::Operator_LessThan     ));
        ts->addStringToken("<=",              TokenType::Symbol,  i32(TokenSubType::Operator_LessThanEq   ));
        ts->addStringToken(">",               TokenType::Symbol,  i32(TokenSubType::Operator_GreaterThan  ));
        ts->addStringToken(">=",              TokenType::Symbol,  i32(TokenSubType::Operator_GreaterThanEq));
        ts->addStringToken("null",            TokenType::Literal, i32(TokenSubType::Literal_Null          ));
        ts->addStringToken("true",            TokenType::Literal, i32(TokenSubType::Literal_True          ));
        ts->addStringToken("false",           TokenType::Literal, i32(TokenSubType::Literal_False         ));
        ts->addStringToken(";",               TokenType::EndOfStatement);

        ts->addCustomToken(matchTemplateString);

        ts->addRegexToken(IDENTIFIER_REGEX,   TokenType::Identifier                                        );
        ts->addRegexToken(STRING_REGEX,       TokenType::Literal, i32(TokenSubType::Literal_String        ));
        ts->addRegexToken(FLOAT_REGEX"f",     TokenType::Literal, i32(TokenSubType::Literal_Float32       ));
        ts->addRegexToken(FLOAT_REGEX"d?",    TokenType::Literal, i32(TokenSubType::Literal_Float64       ));
        ts->addRegexToken(INT_REGEX"ull",     TokenType::Literal, i32(TokenSubType::Literal_UInt64        ));
        ts->addRegexToken(INT_REGEX"ul",      TokenType::Literal, i32(TokenSubType::Literal_UInt32        ));
        ts->addRegexToken(INT_REGEX"us",      TokenType::Literal, i32(TokenSubType::Literal_UInt16        ));
        ts->addRegexToken(INT_REGEX"ub",      TokenType::Literal, i32(TokenSubType::Literal_UInt8         ));
        ts->addRegexToken(INT_REGEX"ll",      TokenType::Literal, i32(TokenSubType::Literal_Int64         ));
        ts->addRegexToken(INT_REGEX"s",       TokenType::Literal, i32(TokenSubType::Literal_Int16         ));
        ts->addRegexToken(INT_REGEX"b",       TokenType::Literal, i32(TokenSubType::Literal_Int8          ));
        ts->addRegexToken(INT_REGEX"l?",      TokenType::Literal, i32(TokenSubType::Literal_Int32         ));
    }

    u32 parseEscapeSequence(const char* sequence, String& out, Context* ctx) {
        switch (*sequence) {
            case 'n' : { out += '\n'; return 1; }
            case 't' : { out += '\t'; return 1; }
            case 'r' : { out += '\r'; return 1; }
            case 'a' : { out += '\a'; return 1; }
            case 'b' : { out += '\b'; return 1; }
            case '`' : { out += '`' ; return 1; }
            case '$' : { out += '$' ; return 1; }
            case '"' : { out += '\"'; return 1; }
            case '\'': { out += '\''; return 1; }
            case '\\': { out += '\\'; return 1; }
        }

        ctx->logWarn("Invalid escape sequence '\\%c'", *sequence);
        return 0;
    }
};
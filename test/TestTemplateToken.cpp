#include "Common.h"
#include <parse/Tokens.h>
#include <tokenize/TokenSet.h>

using namespace tokenize;
using namespace parse; 

TEST_CASE("Test Template String Matching", "[parse]") {
    SECTION("Matches with a basic string with no embeds") {
        MatchedToken m;
        MatchResult result = matchTemplateString("` `", &m);
        REQUIRE(result == MatchResult::Matched);
        REQUIRE(m.type == TokenType::Literal);
        REQUIRE(m.subType == i32(TokenSubType::Literal_TemplateString));
        REQUIRE(m.offset == 0);
        REQUIRE(m.contentBeginOffset == 1);
        REQUIRE(m.contentEndOffset == 2);
        REQUIRE(m.length == 3);
    }

    SECTION("Matches strings with escaped backticks") {
        MatchedToken m;
        MatchResult result = matchTemplateString("`\\`\\``", &m);
        REQUIRE(result == MatchResult::Matched);
        REQUIRE(m.type == TokenType::Literal);
        REQUIRE(m.subType == i32(TokenSubType::Literal_TemplateString));
        REQUIRE(m.offset == 0);
        REQUIRE(m.contentBeginOffset == 1);
        REQUIRE(m.contentEndOffset == 5);
        REQUIRE(m.length == 6);
    }

    SECTION("Returns EndNotMatched when there's no closing backtick") {
        MatchedToken m;
        MatchResult result = matchTemplateString("`\\`\\`", &m);
        REQUIRE(result == MatchResult::EndNotMatched);
        REQUIRE(m.type == TokenType::Literal);
        REQUIRE(m.subType == i32(TokenSubType::Literal_TemplateString));
        REQUIRE(m.offset == 0);
        REQUIRE(m.contentBeginOffset == 1);
        REQUIRE(m.contentEndOffset == 5);
        REQUIRE(m.length == 5);
    }

    SECTION("Matches with a string that has one embedded value") {
        MatchedToken m;
        MatchResult result;
        result = matchTemplateString("`${value1}`", &m);
        REQUIRE(result == MatchResult::Matched);
        REQUIRE(m.type == TokenType::Literal);
        REQUIRE(m.subType == i32(TokenSubType::Literal_TemplateString));
        REQUIRE(m.offset == 0);
        REQUIRE(m.contentBeginOffset == 1);
        REQUIRE(m.contentEndOffset == 10);
        REQUIRE(m.length == 11);

        result = matchTemplateString("` ${value1} ` ", &m);
        REQUIRE(result == MatchResult::Matched);
        REQUIRE(m.type == TokenType::Literal);
        REQUIRE(m.subType == i32(TokenSubType::Literal_TemplateString));
        REQUIRE(m.offset == 0);
        REQUIRE(m.contentBeginOffset == 1);
        REQUIRE(m.contentEndOffset == 12);
        REQUIRE(m.length == 13);
    }

    SECTION("Matches with a string that has multiple embedded values") {
        MatchedToken m;
        MatchResult result;
        
        result = matchTemplateString("`${value1} ${value2}`", &m);
        REQUIRE(result == MatchResult::Matched);
        REQUIRE(m.type == TokenType::Literal);
        REQUIRE(m.subType == i32(TokenSubType::Literal_TemplateString));
        REQUIRE(m.offset == 0);
        REQUIRE(m.contentBeginOffset == 1);
        REQUIRE(m.contentEndOffset == 20);
        REQUIRE(m.length == 21);

        result = matchTemplateString("` ${value1}${value2} ` ", &m);
        REQUIRE(result == MatchResult::Matched);
        REQUIRE(m.type == TokenType::Literal);
        REQUIRE(m.subType == i32(TokenSubType::Literal_TemplateString));
        REQUIRE(m.offset == 0);
        REQUIRE(m.contentBeginOffset == 1);
        REQUIRE(m.contentEndOffset == 21);
        REQUIRE(m.length == 22);
    }

    SECTION("Matches with a string that has embedded values that contain braces and nested strings") {
        MatchedToken m;
        MatchResult result;
        
        result = matchTemplateString("`${{}{{}}} ${`${a}`}`", &m);
        REQUIRE(result == MatchResult::Matched);
        REQUIRE(m.type == TokenType::Literal);
        REQUIRE(m.subType == i32(TokenSubType::Literal_TemplateString));
        REQUIRE(m.offset == 0);
        REQUIRE(m.contentBeginOffset == 1);
        REQUIRE(m.contentEndOffset == 20);
        REQUIRE(m.length == 21);
    }
}
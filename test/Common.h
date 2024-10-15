#include <catch2/catch_test_macros.hpp>
#include <parse/Context.h>
#include <parse/Tokens.h>
#include <tokenize/TokenizedSource.h>
#include <tokenize/Resource.h>
#include <tokenize/TokenSet.h>

void setupTest();

class TestContext {
    public:
        TestContext(const char* source);
        ~TestContext();

        parse::Context* ctx;
        tokenize::TokenSet* tokenSet;
        tokenize::Resource* src;
        tokenize::TokenizedSource* tokenizedSrc;
};
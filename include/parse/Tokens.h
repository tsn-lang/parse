#pragma once
#include <parse/types.h>

namespace utils {
    class String;
};

namespace tokenize {
    class TokenSet;
    enum class MatchResult;
    struct MatchedToken;
};

namespace parse {
    class Context;

    // Expects to be called at this position in the string:
    // `example ${string} value`
    // `.........^`
    //
    // Returns a pointer to this position in the string
    // `example ${string} value`
    // `................^`
    const char* findEndOfTemplateLiteralEmbeddedValue(const char* input);
    MatchResult matchTemplateString(const char* input, MatchedToken* outMatch);

    void setupTokenSet(TokenSet* ts);
    u32 parseEscapeSequence(const char* sequence, String& out, Context* ctx);
};
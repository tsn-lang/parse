#pragma once
#include <parse/types.h>
#include <tokenize/SourceLocation.h>
#include <utils/Array.h>
#include <utils/String.h>
#include <utils/MemoryPool.h>
#include <utils/interfaces/IWithLogging.h>
#include <utils/interfaces/ILogHandler.h>

namespace tokenize {
    class TokenizedSource;
    struct Token;
};

namespace parse {
    class Context;
    class Node;

    class Match {
        public:
            Match(TokenType type);
            Match(TokenType type, TokenSubType subType, const char* exactText = nullptr);
            Match(TokenType type, const char* exactText, TokenSubType subType = TokenSubType::None);

            TokenType type;
            TokenSubType subType;
            const char* exactText;
    };

    struct ParseMessage {
        enum class Type {
            Debug,
            Info,
            Warning,
            Error
        };

        Type type;
        String message;
        SourceLocation location;
    };

    class ParseState : public ILogHandler {
        public:
            ParseState(Context* ctx);
            void commit(const ParseState& state);

            virtual void onDebug(const char* msg);
            virtual void onInfo(const char* msg);
            virtual void onWarn(const char* msg);
            virtual void onError(const char* msg);

            Context* ctx;
            u32 tokenIndex;
            Array<ParseMessage> messages;
    };

    class Context : public IWithLogging {
        public:
            Context(TokenizedSource* source);

            void* allocNode();
            void freeNode(void* node);
            TokenizedSource* getTokenizedSource() const;
            ParseState* getState() const;

            void begin();
            void commit();
            void rollback();
            void consume(Node* forNode = nullptr);

            bool atEnd() const;
            const Token* get(i32 offset = 0) const;
            
            bool match(TokenType type) const;
            bool match(TokenType type, TokenSubType subType, const char* exactText = nullptr) const;
            bool match(TokenType type, const char* exactText, TokenSubType subType = TokenSubType::None) const;
            u32 matchAny(const std::initializer_list<Match>& matchList);
            bool matchAll(const std::initializer_list<Match>& matchList);
            
            bool skipTo(TokenType type) const;
            bool skipTo(TokenType type, TokenSubType subType, const char* exactText = nullptr) const;
            bool skipTo(TokenType type, const char* exactText, TokenSubType subType = TokenSubType::None) const;
            bool skipToAnyMatched(const std::initializer_list<Match>& matchList);
            bool skipToAllMatched(const std::initializer_list<Match>& matchList);

        protected:
            MemoryPool m_nodePool;
            TokenizedSource* m_source;
            const Array<Token*>& m_tokens;
            Array<ParseState> m_state;
            ParseState* m_curState;
    };
};
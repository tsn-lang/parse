#include <parse/Context.h>
#include <tokenize/TokenizedSource.h>
#include <tokenize/Token.h>
#include <utils/Array.hpp>
#include <utils/Exception.h>

#include <parse/Node.h>
#include <parse/ErrorNode.h>
#include <parse/ModuleNode.h>
#include <parse/TopLevelNode.h>
#include <parse/StatementNode.h>
#include <parse/FunctionNode.h>
#include <parse/ClassNode.h>
#include <parse/ClassPropertyNode.h>
#include <parse/ClassMethodNode.h>
#include <parse/ClassConstructorNode.h>
#include <parse/ClassDestructorNode.h>
#include <parse/ClassOperatorMethodNode.h>
#include <parse/ClassPropertyGetterNode.h>
#include <parse/ClassPropertySetterNode.h>
#include <parse/TypeNode.h>
#include <parse/EnumNode.h>
#include <parse/IdentifierNode.h>
#include <parse/NumberLiteralNode.h>
#include <parse/BooleanLiteralNode.h>
#include <parse/StringLiteralNode.h>
#include <parse/ExpressionNode.h>
#include <parse/TypedAssignableNode.h>
#include <parse/StatementBlockNode.h>
#include <parse/AliasTypeNode.h>
#include <parse/FunctionTypeNode.h>
#include <parse/ObjectTypeNode.h>
#include <parse/ObjectTypePropertyNode.h>
#include <parse/ArrayTypeNode.h>
#include <parse/ArrayTypeElementNode.h>
#include <parse/TypeSpecifierNode.h>

namespace parse {
    Match::Match(TokenType _type)
        : type(_type), subType(TokenSubType::None), exactText(nullptr) {}
    Match::Match(TokenType _type, TokenSubType _subType, const char* _exactText) 
        : type(_type), subType(_subType), exactText(_exactText) {}
    Match::Match(TokenType _type, const char* _exactText, TokenSubType _subType)
        : type(_type), subType(_subType), exactText(_exactText) {}
    
    u32 MaxNodeSize() {
        u32 maxSize = 0;

        #define N(name) if (sizeof(name) > maxSize) maxSize = sizeof(name)

        N(Node);
        N(ErrorNode);
        N(ModuleNode);
        N(TopLevelNode);
        N(StatementNode);
        N(FunctionNode);
        N(ClassNode);
        N(ClassPropertyNode);
        N(ClassMethodNode);
        N(ClassConstructorNode);
        N(ClassDestructorNode);
        N(ClassOperatorMethodNode);
        N(ClassPropertyGetterNode);
        N(ClassPropertySetterNode);
        N(TypeNode);
        N(EnumNode);
        N(IdentifierNode);
        N(NumberLiteralNode);
        N(BooleanLiteralNode);
        N(StringLiteralNode);
        N(ExpressionNode);
        N(TypedAssignableNode);
        N(StatementBlockNode);
        N(AliasTypeNode);
        N(FunctionTypeNode);
        N(ObjectTypeNode);
        N(ObjectTypePropertyNode);
        N(ArrayTypeNode);
        N(ArrayTypeElementNode);
        N(TypeSpecifierNode);

        #undef N

        return maxSize;
    }



    ParseState::ParseState(Context* _ctx) {
        ctx = _ctx;
        tokenIndex = 0;
    }

    void ParseState::commit(const ParseState& state) {
        tokenIndex = state.tokenIndex;
        messages.append(state.messages);
    }

    void ParseState::onDebug(const char* msg) {
        const Token* tok = ctx->get();
        messages.push({
            ParseMessage::Type::Debug,
            String(msg),
            tok ? tok->location : SourceLocation({ 0, 0, 0, 0, 0, 0, 0 })
        });
    }

    void ParseState::onInfo(const char* msg) {
        const Token* tok = ctx->get();
        messages.push({
            ParseMessage::Type::Info,
            String(msg),
            tok ? tok->location : SourceLocation({ 0, 0, 0, 0, 0, 0, 0 })
        });
    }

    void ParseState::onWarn(const char* msg) {
        const Token* tok = ctx->get();
        messages.push({
            ParseMessage::Type::Warning,
            String(msg),
            tok ? tok->location : SourceLocation({ 0, 0, 0, 0, 0, 0, 0 })
        });
    }

    void ParseState::onError(const char* msg) {
        const Token* tok = ctx->get();
        messages.push({
            ParseMessage::Type::Error,
            String(msg),
            tok ? tok->location : SourceLocation({ 0, 0, 0, 0, 0, 0, 0 })
        });
    }


    
    Context::Context(TokenizedSource* source)
        : m_nodePool(MaxNodeSize(), 128, false), m_source(source), m_tokens(source->getTokens()), m_curState(nullptr)
    {
        begin();
    }

    void* Context::allocNode() {
        return m_nodePool.alloc();
    }

    void Context::freeNode(void* node) {
        m_nodePool.free(node);
    }

    void Context::begin() {
        m_state.push(m_curState ? *m_curState : ParseState(this));
        m_curState = &m_state.last();
        setLogHandler(m_curState);
    }

    void Context::commit() {
        if (m_state.size() == 1) throw Exception("Context::commit - Cannot commit without first calling Context::begin");
        m_state[m_state.size() - 2].commit(*m_curState);
        m_state.pop();
        m_curState = &m_state.last();
        setLogHandler(m_curState);
    }

    void Context::rollback() {
        if (m_state.size() == 1) throw Exception("Context::rollback - Cannot rollback without first calling Context::begin");
        m_state.pop();
        m_curState = &m_state.last();
        setLogHandler(m_curState);
    }

    void Context::consume() {
        u32 nextIdx = m_curState->tokenIndex + 1;
        if (nextIdx == m_tokens.size()) return;
        m_curState->tokenIndex = nextIdx;
    }

    bool Context::atEnd() const {
        if (m_curState->tokenIndex == m_tokens.size()) return true;
        return m_tokens[m_curState->tokenIndex]->type == TokenType::EndOfInput;
    }

    const Token* Context::get(i32 offset) const {
        i64 index = i32(m_curState->tokenIndex) + offset;
        if (index < 0 || index >= m_tokens.size()) return nullptr;
        return m_tokens[u32(index)];
    }

    bool Context::match(TokenType type) const {
        if (m_tokens.size() == 0) return type == TokenType::EndOfInput;

        const Token* tok = m_tokens[m_curState->tokenIndex];
        return tok->type == type;
    }

    bool Context::match(TokenType type, TokenSubType subType, const char* exactText) const {
        if (m_tokens.size() == 0) return type == TokenType::EndOfInput;

        const Token* tok = m_tokens[m_curState->tokenIndex];
        return tok->type == type && tok->subType == i32(subType) && (!exactText || tok->toString() == exactText);
    }

    bool Context::match(TokenType type, const char* exactText, TokenSubType subType) const {
        if (m_tokens.size() == 0) return type == TokenType::EndOfInput;

        const Token* tok = m_tokens[m_curState->tokenIndex];
        return tok->type == type && tok->subType == i32(subType) && (!exactText || tok->toString() == exactText);
    }

    u32 Context::matchAny(const std::initializer_list<Match>& matchList) {
        if (m_tokens.size() == 0) {
            u32 idx = 0;
            for (const Match& m : matchList) {
                if (m.type == TokenType::EndOfInput) return idx + 1;
                idx++;
            }

            return 0;
        }

        const Token* tok = m_tokens[m_curState->tokenIndex];

        u32 idx = 0;
        for (const Match& m : matchList) {
            if (tok->type == m.type && tok->subType == i32(m.subType) && (!m.exactText || tok->toString() == m.exactText)) {
                return idx + 1;
            }
            idx++;
        }

        return 0;
    }

    bool Context::matchAll(const std::initializer_list<Match>& matchList) {
        if (m_tokens.size() == 0) {
            if (matchList.size() != 1) return false;
            return matchList.begin()->type == TokenType::EndOfInput;
        }

        if (matchList.size() + m_curState->tokenIndex >= m_tokens.size()) return false;
        
        u32 idx = m_curState->tokenIndex;
        for (const Match& m : matchList) {
            const Token* tok = m_tokens[idx];
            if (tok->type != m.type || tok->subType != i32(m.subType) || (m.exactText && tok->toString() != m.exactText)) {
                return false;
            }
        }

        return true;
    }

    bool Context::skipTo(TokenType type) const {
        if (m_tokens.size() == 0) return type == TokenType::EndOfInput;

        u32 idx = m_curState->tokenIndex;
        while (idx < m_tokens.size()) {
            const Token* tok = m_tokens[idx];
            if (tok->type == type || tok->type == TokenType::EndOfInput) {
                m_curState->tokenIndex = idx;
                return true;
            }

            idx++;
        }

        m_curState->tokenIndex = idx;
        return false;
    }

    bool Context::skipTo(TokenType type, TokenSubType subType, const char* exactText) const {
        if (m_tokens.size() == 0) return type == TokenType::EndOfInput;

        u32 idx = m_curState->tokenIndex;
        while (idx < m_tokens.size()) {
            const Token* tok = m_tokens[idx];
            if (tok->type == TokenType::EndOfInput) {
                m_curState->tokenIndex = idx;
                return true;
            }

            if (tok->type == type && tok->subType == i32(subType) && (!exactText || tok->toString() == exactText)) {
                m_curState->tokenIndex = idx;
                return true;
            }

            idx++;
        }

        m_curState->tokenIndex = idx;
        return false;
    }

    bool Context::skipTo(TokenType type, const char* exactText, TokenSubType subType) const {
        if (m_tokens.size() == 0) return type == TokenType::EndOfInput;

        u32 idx = m_curState->tokenIndex;
        while (idx < m_tokens.size()) {
            const Token* tok = m_tokens[idx];
            if (tok->type == TokenType::EndOfInput) {
                m_curState->tokenIndex = idx;
                return true;
            }

            if (tok->type == type && tok->subType == i32(subType) && (!exactText || tok->toString() == exactText)) {
                m_curState->tokenIndex = idx;
                return true;
            }

            idx++;
        }

        m_curState->tokenIndex = idx;
        return false;
    }

    bool Context::skipToAnyMatched(const std::initializer_list<Match>& matchList) {
        if (m_tokens.size() == 0) {
            for (const Match& m : matchList) {
                if (m.type == TokenType::EndOfInput) return true;
            }
            return false;
        }

        while (m_curState->tokenIndex < m_tokens.size()) {
            if (matchAny(matchList)) return true;
            m_curState->tokenIndex++;
        }

        return false;
    }

    bool Context::skipToAllMatched(const std::initializer_list<Match>& matchList) {
        if (m_tokens.size() == 0) {
            if (matchList.size() == 1 && matchList.begin()->type == TokenType::EndOfInput) return true;
            return false;
        }

        while (m_curState->tokenIndex < m_tokens.size()) {
            if (matchAll(matchList)) return true;
            m_curState->tokenIndex++;
        }

        return false;
    }
};
#include <parse/Context.h>
#include <tokenize/TokenizedSource.h>
#include <tokenize/Token.h>
#include <utils/Array.hpp>
#include <utils/Exception.h>

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
    Match::Match(TokenType _type)
        : type(_type), subType(TokenSubType::None), exactText(nullptr) {}
    Match::Match(TokenType _type, TokenSubType _subType, const char* _exactText) 
        : type(_type), subType(_subType), exactText(_exactText) {}
    Match::Match(TokenType _type, const char* _exactText, TokenSubType _subType)
        : type(_type), subType(_subType), exactText(_exactText) {}
    
    u32 MaxNodeSize() {
        u32 maxSize = 0;

        #define N(name) if (sizeof(name) > maxSize) maxSize = sizeof(name)

        N(ArrayTypeNode);
        N(ArrowFunctionExpressionNode);
        N(AwaitExpressionNode);
        N(BinaryExpressionNode);
        N(BooleanLiteralNode);
        N(BreakStatementNode);
        N(CallExpressionNode);
        N(CastExpressionNode);
        N(CatchStatementNode);
        N(ClassConstructorNode);
        N(ClassDestructorNode);
        N(ClassMethodNode);
        N(ClassNode);
        N(ClassOperatorMethodNode);
        N(ClassPropertyGetterNode);
        N(ClassPropertyNode);
        N(ClassPropertySetterNode);
        N(ConditionalExpressionNode);
        N(ContinueStatementNode);
        N(DeclarationStatementNode);
        N(DeleteStatementNode);
        N(DoWhileLoopNode);
        N(EmptyStatementNode);
        N(EnumFieldNode);
        N(EnumNode);
        N(ExportStatementNode);
        N(ExpressionNode);
        N(ExpressionSequenceNode);
        N(ExpressionStatementNode);
        N(ForLoopNode);
        N(FunctionExpressionNode);
        N(FunctionNode);
        N(FunctionTypeNode);
        N(IdentifierNode);
        N(IdentifierTypeSpecifierNode);
        N(IfStatementNode);
        N(IndexExpressionNode);
        N(ImportAllStatementNode);
        N(ImportSelectStatementNode);
        N(MemberExpressionNode);
        N(ModuleNode);
        N(NewExpressionNode);
        N(Node);
        N(NullLiteralNode);
        N(NumberLiteralNode);
        N(ObjectTypeNode);
        N(ParameterListNode);
        N(ReturnStatementNode);
        N(SizeOfExpressionNode);
        N(StatementBlockNode);
        N(StatementNode);
        N(StringLiteralNode);
        N(SwitchCaseNode);
        N(SwitchStatementNode);
        N(TemplateStringLiteralNode);
        N(ThisExpressionNode);
        N(ThrowStatementNode);
        N(TopLevelNode);
        N(TryCatchStatementNode);
        N(TypedAssignableNode);
        N(TypeInfoExpressionNode);
        N(TypeNode);
        N(TypeParameterNode);
        N(TypeSpecifierNode);
        N(UnaryExpressionNode);
        N(WhileLoopNode);

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
        ((Node*)node)->~Node();
        m_nodePool.free(node);
    }
    
    TokenizedSource* Context::getTokenizedSource() const {
        return m_source;
    }
    
    ParseState* Context::getState() const {
        return m_curState;
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

    void Context::consume(Node* forNode) {
        if (forNode) forNode->extendLocation(m_tokens[m_curState->tokenIndex]);

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
            const Token* tok = m_tokens[idx++];
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
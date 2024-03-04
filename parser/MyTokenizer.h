#ifndef _MY_TOKENIZER_H_
#define _MY_TOKENIZER_H_

#include "../../common/MyStringA.h"

#define TOKEN_EOF                     0
#define TOKEN_TYPE_REF                1
#define TOKEN_IDENTIFIER              2
#define TOKEN_ONE_LINE_COMMENT        3
#define TOKEN_MUL_LINE_COMMENT        4
#define TOKEN_NUMBER                  5
#define TOKEN_REAL_NUMBER             6
#define TOKEN_BSTRING                 7
#define TOKEN_XMLBSTRING              8
#define TOKEN_HEXSTRING               9
#define TOKEN_XMLHEXSTRING            10
#define TOKEN_CSTRING                 11
#define TOKEN_XMLCSTRING              12
#define TOKEN_ASSIGNMENT              13
#define TOKEN_RANGE_SEPARATOR         14
#define TOKEN_ELLIPSIS                15
#define TOKEN_LEFT_VERSION_BRACKETS   16
#define TOKEN_RIGHT_VERSION_BRACKETS  17
#define TOKEN_XML_END_TAG_START       18
#define TOKEN_XML_SIGNAL_TAG_END      19
#define TOKEN_XML_BOOL_FALSE          20
#define TOKEN_XML_BOOL_TRUE           21
#define TOKEN_XML_ASN1_TYPE_NAME      22
#define TOKEN_SINGLE_CHAR_ITEM        23
#define TOKEN_RESERVED_WORD           24

#define NO_MORE_CHAR     -1

class MyTokenizer {
public:
	MyTokenizer(const char* data, int lenData) {
		Reset();
		m_Content.Set(data, lenData);
	}

	bool HasMore() { return m_Offset < m_Content.Length(); }
	int Next();
	int Peek();


	MyStringA* Token() { return &m_Token; }
	int        TokenType() { return m_TokenType; }

	void Reset() {
		m_Content.Reset();
		m_Offset = 0;
		m_Token.Reset();
		m_TokenType = 0;
		m_Peek = false;
	}

private:
	bool HasMoreChar() { return m_Offset < m_Content.Length(); }
	bool IsNextWhitespace() { return IsWhitespace(m_Content.CharAt(m_Offset)); }
	int ExpectNextChar(char c, const char* errMsg);

	int ParseTypeRef();
	int ParseIdentifier();
	int ParseSingleLineComment();
	int ParseMultipleLineComment();
	int ParseNumber();
	int ParseBStringOrHexString();
	int ParseCString();

	bool IsSingleCharLexicalItem(char c);

	static bool IsWhitespace(char c) { return c == 9 || c == 10 || c == 11 || c == 12 || c == 13 || c == 32; }
	static bool IsNewLine(char c) { return c == 10 || c == 11 || c == 12 || c == 13; }
	static bool IsLowerCaseLetter(char c) { return c >= 'a' && c <= 'z'; }
	static bool IsUpperCaseLetter(char c) { return c >= 'A' && c <= 'Z'; }
	static bool IsLetter(char c) { return IsLowerCaseLetter(c) || IsUpperCaseLetter(c); }
	static bool IsDigit(char c) { return  c >= '0' && c <= '9'; }

private:
	MyStringA m_Content;
	int       m_Offset;

	MyStringA m_Token;
	int       m_TokenType;
	bool      m_Peek;

	MY_LAST_ERROR_DECL;
};


#endif // _MY_TOKENIZER_H_
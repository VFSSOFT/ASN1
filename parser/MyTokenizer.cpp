#ifndef _MY_TOKENIZER_CPP_
#define _MY_TOKENIZER_CPP_

#include "MyTokenizer.h"


int MyTokenizer::Next() {
	int err = 0;

	if (m_Peek) {
		m_Peek = false;
		return 0;
	}

	m_Token.Reset();
	m_TokenType = TOKEN_EOF;

	int startOff = m_Offset;

	if (!HasMoreChar()) {
		return 0;
	}

	char c = m_Content.CharAt(m_Offset++);
	while (IsWhitespace(c)) { // SKip all whitesapces
		if (HasMoreChar()) {
			c = m_Content.CharAt(m_Offset++);
		} else {
			return 0;
		}
	}

	if (IsUpperCaseLetter(c)) {
		if (err = ParseTypeRef()) return err;
	} else if (IsLowerCaseLetter(c)) {
		if (err = ParseIdentifier()) return err;
	} else if (c == '-') {
		if (!HasMoreChar() || IsNextWhitespace()) {
			m_TokenType = TOKEN_SINGLE_CHAR_ITEM;
			m_Token.Set("-");
		} else {
			if (err = ExpectNextChar('-', "One line comment")) return err;
			if (err = ParseSingleLineComment()) return err;
		}
	} else if (c == '/') {
		c = m_Content.CharAt(m_Offset++);
		if (c == '>') {
			m_TokenType = TOKEN_XML_SIGNAL_TAG_END;
			m_Token.Set("/>");
		} else {
			if (c != '*') return LastError(MY_ERR_INVALID_PARAMETERS, "Invalid ASN.1 Schema: Multiple line comment");
			if (err = ParseMultipleLineComment()) return err;
		}
	} else if (IsDigit(c)) {
		if (err = ParseNumber()) return err;
	} else if (c == '\'') {
		if (!HasMoreChar() || IsNextWhitespace()) {
			m_TokenType = TOKEN_SINGLE_CHAR_ITEM;
			m_Token.Set("\'");
		} else {
			if (err = ParseBStringOrHexString()) return err;
		}
	} else if (c == '\"') {
		if (!HasMoreChar() || IsNextWhitespace()) {
			m_TokenType = TOKEN_SINGLE_CHAR_ITEM;
			m_Token.Set("\"");
		} else {
			if (err = ParseCString()) return err;
		}
	} else if (c == ':') {
		if (!HasMoreChar() || IsNextWhitespace()) {
			m_TokenType = TOKEN_SINGLE_CHAR_ITEM;
			m_Token.Set(":");
		} else {
			if (err = ExpectNextChar(':', "Expect Assignment lexical item")) return err;
			if (err = ExpectNextChar('=', "Expect Assignment lexical item")) return err;
			m_TokenType = TOKEN_ASSIGNMENT;
			m_Token.Set("::=");
		}
	} else if (c == '.') {
		if (!HasMoreChar() || IsNextWhitespace()) {
			m_TokenType = TOKEN_SINGLE_CHAR_ITEM;
			m_Token.Set(".");
		} else {
			if (err = ExpectNextChar('.', "Expect Range/Ellipsis lexical item")) return err;

			char nc = m_Content.CharAt(m_Offset);
			if (nc == '.') {
				m_Offset++;
				m_TokenType = TOKEN_ELLIPSIS;
				m_Token.Set("...");
			} else {
				m_TokenType = TOKEN_RANGE_SEPARATOR;
				m_Token.Set("..");
			}
		}
	} else if (c == '[') {
		if (!HasMoreChar() || IsNextWhitespace()) {
			m_TokenType = TOKEN_SINGLE_CHAR_ITEM;
			m_Token.Set("[");
		} else {
			if (err = ExpectNextChar('[', "Expect Left version brackets")) return err;
			m_TokenType = TOKEN_LEFT_VERSION_BRACKETS;
			m_Token.Set("[[");
		}
	} else if (c == ']') {
		if (!HasMoreChar() || IsNextWhitespace()) {
			m_TokenType = TOKEN_SINGLE_CHAR_ITEM;
			m_Token.Set("]");
		} else {
			if (err = ExpectNextChar(']', "Expect Right version brackets")) return err;
			m_TokenType = TOKEN_RIGHT_VERSION_BRACKETS;
			m_Token.Set("]]");
		}
	} else if (c == '<') {
		if (!HasMoreChar() || IsNextWhitespace()) {
			m_TokenType = TOKEN_SINGLE_CHAR_ITEM;
			m_Token.Set("<");
		} else {
			if (err = ExpectNextChar('/', "Expect XML end tag start item")) return err;
			m_TokenType = TOKEN_XML_END_TAG_START;
			m_Token.Set("</");
		}
	} else if (IsSingleCharLexicalItem(c)) {
		m_TokenType = TOKEN_SINGLE_CHAR_ITEM;
		m_Token.AppendChar(c);
	}
	return err;
}

int MyTokenizer::Peek() {
	int err = 0;
	
	if (m_Peek) return 0;

	if (err = Next()) return err;

	m_Peek = m_TokenType != TOKEN_EOF;
	return 0;
}

int MyTokenizer::ParseTypeRef() {
	m_TokenType = TOKEN_TYPE_REF;
	m_Token.Reset();
	m_Token.AppendChar(m_Content.CharAt(m_Offset-1));

	while (m_Offset < m_Content.Length()) {
		char c = m_Content.CharAt(m_Offset);
		if (IsLetter(c) || IsDigit(c) || c == '-') { 
			m_Offset++;
			m_Token.AppendChar(c);
		} else {
			break;
		}
	}

	return 0;
}
int MyTokenizer::ParseIdentifier() {
	m_TokenType = TOKEN_IDENTIFIER;
	m_Token.Reset();
	m_Token.AppendChar(m_Content.CharAt(m_Offset-1));

	while (m_Offset < m_Content.Length()) {
		char c = m_Content.CharAt(m_Offset);
		if (IsLetter(c) || IsDigit(c) || c == '-') { 
			m_Offset++;
			m_Token.AppendChar(c);
		} else {
			break;
		}
	}

	return 0;
}
int MyTokenizer::ParseSingleLineComment() {
	m_TokenType = TOKEN_ONE_LINE_COMMENT;
	m_Token.Reset();

	char prevChar = 0;
	while (m_Offset < m_Content.Length()) {
		char c = m_Content.CharAt(m_Offset++);
		
		if (IsNewLine(c)) {
			break; // comment end
		}
		
		if (prevChar == '-' && c == '-') {
			m_Token.SetLength(m_Token.Length() - 1); // remove last '-'
			break; // comment end
		}

		m_Token.AppendChar(c);
		prevChar = c;
	}

	return 0;
}
int MyTokenizer::ParseMultipleLineComment() {
	m_TokenType = TOKEN_MUL_LINE_COMMENT;
	m_Token.Reset();

	char prevChar = 0;
	int nestedCount = 1; // For each "/*", there should be a "*/" matched with them.
	while (m_Offset < m_Content.Length()) {
		char c = m_Content.CharAt(m_Offset++);

		if (prevChar == '*' && c == '/') {
			nestedCount--;
			if (nestedCount == 0) {
				m_Token.SetLength(m_Token.Length() - 1);
				break; // done
			}
		} else if (prevChar == '/' && c == '*') {
			nestedCount++;
		}

		m_Token.AppendChar(c);
		prevChar = c;
	}
	
	return 0;
}
int MyTokenizer::ParseNumber() {
	m_TokenType = TOKEN_NUMBER;
	m_Token.Reset();
	m_Token.AppendChar(m_Content.CharAt(m_Offset-1));

	while (m_Offset < m_Content.Length()) {
		char c = m_Content.CharAt(m_Offset);

		if (IsDigit(c) || c == '.' || c == 'e' || c == 'E') {
			if (c == '.' || c == 'e' || c == 'E') m_TokenType = TOKEN_REAL_NUMBER;
			m_Offset++;
			m_Token.AppendChar(c);
		} else {
			break;
		}
	}

	return 0;
}
int MyTokenizer::ParseBStringOrHexString() {
	m_Token.Reset();

	while (m_Offset < m_Content.Length()) {
		char c = m_Content.CharAt(m_Offset++);
		if (c != '\'') {
			if (!IsWhitespace(c)) m_Token.AppendChar(c);
		} else {
			c = m_Content.CharAt(m_Offset++);
			if (c == 'B') m_TokenType = TOKEN_BSTRING;
			else if (c == 'H') m_TokenType = TOKEN_HEXSTRING;
			else return LastError(MY_ERR_INVALID_PARAMETERS, "Invalid ASN.1 Schema");
			break;
		}
	}

	return 0;
}
int MyTokenizer::ParseCString() {
	m_TokenType = TOKEN_CSTRING;
	m_Token.Reset();

	bool ignoreWhitespace = false;
	while (m_Offset < m_Content.Length()) {
		char c = m_Content.CharAt(m_Offset++);

		if (c == '\"') {
			char nc = m_Content.CharAt(m_Offset);
			if (nc == '\"') {
				m_Offset++;
				m_Token.AppendChar('\"');
			} else {
				break; // end of the cstring
			}
		} else if (IsNewLine(c)) {
			ignoreWhitespace = true;
		} else {
			if (IsWhitespace(c) && ignoreWhitespace) {
				// ignore it
			} else {
				m_Token.AppendChar(c);
				ignoreWhitespace = false;
			}
		}
	}

	return 0;
}

int MyTokenizer::ExpectNextChar(char c, const char* errMsg) {
	char nc = m_Content.CharAt(m_Offset);
	if (nc != c) {
		m_LastErrorCode = MY_ERR_INVALID_PARAMETERS;
		m_LastErrorMessage.SetWithFormat("Invalid ASN.1 Schema: %s", errMsg);
		return m_LastErrorCode;
	}
	m_Offset++;
	return 0;
}


bool MyTokenizer::IsSingleCharLexicalItem(char c) {
	const char* singleCharLexicalItems = "{}<>,.()[]-:=\"\' ;@|!^";
	for (int i = 0; i < strlen(singleCharLexicalItems); i++) {
		if (c == singleCharLexicalItems[i]) return true;
	}
	return false;
}


#endif // _MY_TOKENIZER_CPP_

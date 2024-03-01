#ifndef _MY_TOKENIZER_CPP_
#define _MY_TOKENIZER_CPP_

#include "MyTokenizer.h"


int MyTokenizer::Next() {
	int err = 0;
	m_Token.Reset();
	m_TokenType = 0;

	int startOff = m_Offset;

	char c = NextChar();
	if (c == NO_MORE_CHAR) { return 0; }

	if (IsUpperCaseLetter(c)) {
		m_TokenType = TOKEN_TYPE_REF;

		while (m_Offset < m_Content.Length()) {
			c = m_Content.CharAt(m_Offset);
			if (IsLetter(c) || IsDigit(c) || c == '-') { 
				m_Offset++;
			} else {
				m_Token.Set(m_Content.DerefConst(startOff), m_Offset-startOff);
				break;
			}
		}
		
	} else if (IsLowerCaseLetter(c)) {
		m_TokenType = TOKEN_IDENTIFIER;

		while (m_Offset < m_Content.Length()) {
			c = m_Content.CharAt(m_Offset);
			if (IsLetter(c) || IsDigit(c) || c == '-') { 
				m_Offset++;
			} else {
				m_Token.Set(m_Content.DerefConst(startOff), m_Offset-startOff);
				break;
			}
		}
		
	} else if (c == '-') {
		c = NextChar();
		if (c != '-') return LastError(MY_ERR_INVALID_PARAMETERS, "Invalid ASN.1 Schema: One line comment");
		
		m_TokenType = TOKEN_ONE_LINE_COMMENT;
		startOff = m_Offset;
		while (m_Offset < m_Content.Length()) {
			c = m_Content.CharAt(m_Offset++);
			if (IsNewLine(c)) {
				m_Token.Set(m_Content.DerefConst(startOff), m_Offset-startOff);
				break; // comment end
			} else if (c == '-') {
				if (m_Content.CharAt(m_Offset) == '-') {
					m_Token.Set(m_Content.DerefConst(startOff), m_Offset-startOff);
					m_Offset++;
					break; // comment end
				}
			}
		}
		
	} else if (c == '/') {
		c = NextChar();
		if (c != '*') return LastError(MY_ERR_INVALID_PARAMETERS, "Invalid ASN.1 Schema: Multiple line comment");

		m_TokenType = TOKEN_MUL_LINE_COMMENT;
		startOff = m_Offset;
		int nestedCount = 1; // For each "/*", there should be a "*/" matched with them.
		while (m_Offset < m_Content.Length()) {
			c = m_Content.CharAt(m_Offset++);
			if (c == '*') {
				char nc = m_Content.CharAt(m_Offset);
				if (nc == '/') {
					nestedCount--;
					m_Offset++;
					if (nestedCount == 0) {
						m_Token.Set(m_Content.DerefConst(startOff), m_Offset-startOff-2);
						break; // done
					}
				}
			} else if (c == '/') {
				char nc = m_Content.CharAt(m_Offset);
				if (nc == '*') {
					nestedCount++;
					m_Offset++;
				}
			}
		}
		
	} else if (IsDigit(c)) {
		m_TokenType = TOKEN_NUMBER;

		while (m_Offset < m_Content.Length()) {
			c = m_Content.CharAt(m_Offset);

			if (IsDigit(c) || c == '.' || c == 'e' || c == 'E') {
				if (c == '.' || c == 'e' || c == 'E') m_TokenType = TOKEN_REAL_NUMBER;
				m_Offset++;
			} else {
				m_Token.Set(m_Content.DerefConst(startOff), m_Offset-startOff);
				break;
			}
		}
	} else if (c == '\'') {
		while (m_Offset < m_Content.Length()) {
			c = m_Content.CharAt(m_Offset);

			if (c != '\'') {
				m_Offset++;
			} else {
				c = NextChar();
				if (c == 'B') m_TokenType = TOKEN_BSTRING;
				else if (c == 'H') m_TokenType = TOKEN_HEXSTRING;
				else return LastError(MY_ERR_INVALID_PARAMETERS, "Invalid ASN.1 Schema");
				m_Token.Set(m_Content.DerefConst(startOff), m_Offset-startOff);
				break;
			}
		}
	} else if (c == '\"') {
		
	}

	
	return err;
}



char MyTokenizer::NextChar() {
	char c = NO_MORE_CHAR;
	if (m_Offset < m_Content.Length()) {
		c = m_Content.CharAt(m_Offset);
		m_Offset++;
	}
	return c;
}


#endif // _MY_TOKENIZER_CPP_

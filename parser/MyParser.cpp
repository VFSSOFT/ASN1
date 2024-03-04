#ifndef _MY_PARSER_CPP_
#define _MY_PARSER_CPP_

#include "MyParser.h"

int MyParser::Parse(const wchar_t* file) {
	int err = 0;

	return 0;
}


int MyParser::ParseModuleDef(MyTokenizer* tokenizer, MyModuleDef* moduleDef) {
	return 0;
}
int MyParser::ParseModuleIdentifier(MyTokenizer* tokenizer, MyModuleID* moduleID) {
	int err = 0;

	if (err = tokenizer->Next()) return LastError(err, tokenizer->LastErrorMessage());
	if (err = ExpectedTokenType(tokenizer, TOKEN_TYPE_REF)) return err;
	moduleID->ModuleReference.Set(tokenizer->Token());

	if (err = ParseDefinitiveIdentifier(tokenizer, &moduleID->DefinitiveID)) return err;

	moduleID->SetValid(true);
	return 0;
}

int MyParser::ParseDefinitiveIdentifier(MyTokenizer* tokenizer, MyDefinitiveID* definitiveID) {
	int err = 0;

	if (err = tokenizer->Peek()) return LastError(err, tokenizer->LastErrorMessage());

	if (tokenizer->TokenType() == TOKEN_SINGLE_CHAR_ITEM && tokenizer->Token()->Equals("{")) {
		definitiveID->Empty = false;

		tokenizer->Next(); // consume '{'
		if (err = ParseDefinitiveObjIdComponentList(tokenizer, &definitiveID->DefinitiveObjIdComponentList)) return err;
		tokenizer->Next(); // consume '}'

	} else {
		definitiveID->Empty = true;
	}

	definitiveID->SetValid(true);
	return 0;
}

int MyParser::ParseDefinitiveObjIdComponentList(MyTokenizer* tokenizer, MyDefinitiveObjIdComponentList* list) {
	int err = 0;

	while (true) {
		if (err = tokenizer->Peek()) return LastError(err, tokenizer->LastErrorMessage());

		if (tokenizer->TokenType() == TOKEN_NUMBER || tokenizer->TokenType() == TOKEN_IDENTIFIER) {
			MyDefinitiveObjIdComponent* comp = list->DefinitiveObjIdComponents.AddNew();
			if (err = ParseDefinitiveObjIdComponent(tokenizer, comp)) return err;
		} else {
			break;
		}
	}

	list->SetValid(true);
	return 0;
}

int MyParser::ParseDefinitiveObjIdComponent(MyTokenizer* tokenizer, MyDefinitiveObjIdComponent* comp) {
	int err = 0;

	if (err = tokenizer->Peek()) return LastError(err, tokenizer->LastErrorMessage());

	if (tokenizer->TokenType() == TOKEN_IDENTIFIER) {
		MyStringA identifier;
		tokenizer->Next();
		identifier.Set(tokenizer->Token());

		if (err = tokenizer->Next()) return LastError(err, tokenizer->LastErrorMessage());

		if (tokenizer->TokenType() == TOKEN_SINGLE_CHAR_ITEM && tokenizer->Token()->Equals("(")) {
			//tokenizer->Next(); // consume "("
			
			comp->DefinitiveNameAndNumberForm.Identifier.Set(&identifier);
			if (err = ParseDefinitiveNumberForm(tokenizer, &comp->DefinitiveNameAndNumberForm.NumberForm)) return err;
			
			tokenizer->Next(); // consume ")"

			comp->DefinitiveNameAndNumberForm.SetValid(true);

		} else {
			comp->NameForm.Set(&identifier);
			comp->NameForm.SetValid(true);
		}

	} else if (tokenizer->TokenType() == TOKEN_NUMBER) {
		if (err = ParseDefinitiveNumberForm(tokenizer, &comp->DefinitiveNumberForm)) return err;
	} else {
		return InvalidToken(TOKEN_IDENTIFIER, TOKEN_NUMBER, tokenizer->TokenType(), "DefinitiveObjIdComponent");
	}

	comp->SetValid(true);
	return 0;
}

int MyParser::ParseNameForm(MyTokenizer* tokenizer, MyNameForm* nameForm) {
	int err = 0;

	if (err = tokenizer->Next()) return LastError(err, tokenizer->LastErrorMessage());
	if (tokenizer->TokenType() != TOKEN_IDENTIFIER) {
		return LastError(MY_ERR_INVALID_PARAMETERS, "Invalid token type: expected Identifier");
	}

	nameForm->Set(tokenizer->Token());
	nameForm->SetValid(true);
	return 0;
}
int MyParser::ParseDefinitiveNumberForm(MyTokenizer* tokenizer, MyDefinitiveNumberForm* definitiveNumberForm) {
	int err = 0;

	if (err = tokenizer->Next()) return LastError(err, tokenizer->LastErrorMessage());
	if (tokenizer->TokenType() != TOKEN_NUMBER) {
		return InvalidToken(TOKEN_NUMBER, tokenizer->TokenType(), "DefinitiveNumberForm");
	}

	definitiveNumberForm->Number.Set(tokenizer->Token());
	definitiveNumberForm->SetValid(true);
	return 0;
}


int MyParser::ExpectedTokenType(MyTokenizer* tokenizer, int tokType) {
	if (tokenizer->TokenType() != tokType) {
		return LastError(MY_ERR_INVALID_PARAMETERS, "Invalid token"); // TODO: more detail information
	}
	return 0;
}

int MyParser::InvalidToken(int expToken, int realToken, const char* defName) {
	m_LastErrorCode = MY_ERR_INVALID_PARAMETERS;
	m_LastErrorMessage.SetWithFormat("Invalid token: expected %d, but is %d", expToken, realToken);
	if (defName) {
		m_LastErrorMessage.Append(", Error Node: ");
		m_LastErrorMessage.Append(defName);
	}
	return m_LastErrorCode;
}
int MyParser::InvalidToken(int expToken1, int expToken2, int realToken, const char* defName) {
	m_LastErrorCode = MY_ERR_INVALID_PARAMETERS;
	m_LastErrorMessage.SetWithFormat("Invalid token: expected %d, %d, but is %d", expToken1, expToken2, realToken);
	if (defName) {
		m_LastErrorMessage.Append(", Error Node: ");
		m_LastErrorMessage.Append(defName);
	}
	return m_LastErrorCode;
}


#endif // _MY_PARSER_CPP_

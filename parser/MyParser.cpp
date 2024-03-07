#ifndef _MY_PARSER_CPP_
#define _MY_PARSER_CPP_

#include "MyParser.h"

#include "../../common/MyFile.h"

int MyParser::Parse(const wchar_t* file) {
	int err = 0;

	MyBuffer data;
	MyFile f;
	if (err = f.ReadAllBytes(file, &data)) return LastError(err, f.LastErrorMessage());

	m_Tokenizer.Set(data.DerefConst(), data.Length());
	if (err = ParseModuleDef(&m_Tokenizer, &m_ModuleDef)) return err;

	return 0;
}


int MyParser::ParseModuleDef(MyTokenizer* tokenizer, MyModuleDef* moduleDef) {
	int err = 0;
	moduleDef->ModuleId = new MyModuleID();
	if (err = ParseModuleIdentifier(tokenizer, moduleDef->ModuleId)) return err;

	if (err = ExpectedReservedWord(tokenizer, "DEFINITIONS")) return err;

	moduleDef->TagDefault = new MyTagDefault();
	if (err = tokenizer->Peek()) return LastError(err, tokenizer->LastErrorMessage());
	if (tokenizer->Token()->Equals("EXPLICIT") || tokenizer->Token()->Equals("IMPLICIT") || tokenizer->Token()->Equals("AUTOMATIC")) {
		if (err = ParseTagDefaults(tokenizer, moduleDef->TagDefault)) return err;
		if (err = tokenizer->Peek()) return LastError(err, tokenizer->LastErrorMessage());
	} else {
		moduleDef->TagDefault->SetValid(true); // Empty
	}

	moduleDef->ExtensionDefault = new MyExtensionDefault();
	if (tokenizer->Token()->Equals("EXTENSIBILITY")) {
		moduleDef->ExtensionDefault->Value.Set("EXTENSIBILITY");
		if (err = tokenizer->Next()) return LastError(err, tokenizer->LastErrorMessage());
		//if (err = tokenizer->Peek()) return LastError(err, tokenizer->LastErrorMessage());
	}
	moduleDef->ExtensionDefault->SetValid(true);

	moduleDef->Body = new MyModuleBody();
	if (err = ExpectedTokenType(tokenizer, TOKEN_ASSIGNMENT)) return err;
	if (err = ExpectedReservedWord(tokenizer, "BEGIN")) return err;
	if (err = ParseModuleBody(tokenizer, moduleDef->Body)) return err;
	if (err = ExpectedReservedWord(tokenizer, "END")) return err;

	moduleDef->SetValid(true);
	return 0;
}
int MyParser::ParseModuleIdentifier(MyTokenizer* tokenizer, MyModuleID* moduleID) {
	int err = 0;
	if (err = ExpectedTokenType(tokenizer, TOKEN_TYPE_REF)) return err;
	moduleID->ModuleReference.Set(tokenizer->Token());

	moduleID->DefinitiveID = new MyDefinitiveID();
	if (err = ParseDefinitiveIdentifier(tokenizer, moduleID->DefinitiveID)) return err;

	moduleID->SetValid(true);
	return 0;
}

int MyParser::ParseDefinitiveIdentifier(MyTokenizer* tokenizer, MyDefinitiveID* definitiveID) {
	int err = 0;

	if (err = tokenizer->Peek()) return LastError(err, tokenizer->LastErrorMessage());

	if (tokenizer->TokenType() == TOKEN_SINGLE_CHAR_ITEM && tokenizer->Token()->Equals("{")) {
		definitiveID->Empty = false;

		tokenizer->Next(); // consume '{'
		definitiveID->DefinitiveObjIdComponentList = new MyDefinitiveObjIdComponentList();
		if (err = ParseDefinitiveObjIdComponentList(tokenizer, definitiveID->DefinitiveObjIdComponentList)) return err;
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
			MyDefinitiveObjIdComponent* comp = new MyDefinitiveObjIdComponent();
			if (err = ParseDefinitiveObjIdComponent(tokenizer, comp)) return err;
			list->DefinitiveObjIdComponents.Add(comp);
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
			
			comp->DefinitiveNameAndNumberForm = new MyDefinitiveNameAndNumberForm();
			comp->DefinitiveNameAndNumberForm->NumberForm = new MyDefinitiveNumberForm();
			comp->DefinitiveNameAndNumberForm->Identifier.Set(&identifier);
			if (err = ParseDefinitiveNumberForm(tokenizer, comp->DefinitiveNameAndNumberForm->NumberForm)) return err;
			
			tokenizer->Next(); // consume ")"

			comp->DefinitiveNameAndNumberForm->SetValid(true);

		} else {
			comp->NameForm = new MyNameForm();
			comp->NameForm->Set(&identifier);
			comp->NameForm->SetValid(true);
		}

	} else if (tokenizer->TokenType() == TOKEN_NUMBER) {
		comp->DefinitiveNameAndNumberForm = new MyDefinitiveNameAndNumberForm();
		if (err = ParseDefinitiveNumberForm(tokenizer, comp->DefinitiveNumberForm)) return err;
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

int MyParser::ParseTagDefaults(MyTokenizer* tokenizer, MyTagDefault* tagDefaults) {
	int err = 0;

	if (err = tokenizer->Next()) return LastError(err, tokenizer->LastErrorMessage());
	if (tokenizer->Token()->Equals("EXPLICIT")) {
		tagDefaults->Value.Set("EXPLICIT");
	} else if (tokenizer->Token()->Equals("IMPLICIT")) {
		tagDefaults->Value.Set("IMPLICIT");
	} else if (tokenizer->Token()->Equals("AUTOMATIC")) {
		tagDefaults->Value.Set("AUTOMATIC");
	} else {
		return InvalidToken(TOKEN_RESERVED_WORD, tokenizer->TokenType(), "TagDefaults");
	}

	if (err = ExpectedReservedWord(tokenizer, "TAGS")) return err;

	tagDefaults->SetValid(true);
	return 0;
}

int MyParser::ParseModuleBody(MyTokenizer* tokenizer, MyModuleBody* body) {
	int err = 0;
	
	body->Exports = new MyExports();
	if (err = Peek(tokenizer)) return err;
	if (tokenizer->Token()->Equals("EXPORTS")) {
		if (err = ParseExports(tokenizer, body->Exports)) return err;
	} else {
		body->Exports->Empty = true;
		body->Exports->SetValid(true);
	}

	body->Imports = new MyImports();
	if (err = Peek(tokenizer)) return err;
	if (tokenizer->Token()->Equals("IMPORTS")) {
		assert(false); // TODO:
	} else {
		body->Imports->Empty = true;
		body->Imports->SetValid(true);
	}



	return 0;
}
int MyParser::ParseExports(MyTokenizer* tokenizer, MyExports* exports) {
	int err = 0;

	if (err = ExpectedReservedWord(tokenizer, "EXPORTS")) return err;
	if (err = Peek(tokenizer)) return err;
	if (tokenizer->Token()->Equals("ALL")) {
		if (err = ExpectedReservedWord(tokenizer, "ALL")) return err;
		if (err = ExpectedTokenType(tokenizer, TOKEN_SINGLE_CHAR_ITEM, ";")) return err;

		exports->ExportAll = true;
	} else {
		assert(false); // TODO:
	}

	exports->SetValid(true);
	return 0;
}
int MyParser::ParseSymbolsExported(MyTokenizer* tokenizer, MySymbolsExported* exported) {
	return 0;
}
int MyParser::ParseSymbolList(MyTokenizer* tokenizer, MySymbolList* symbolList) {
	return 0;
}
int MyParser::ParseSymbol(MyTokenizer* tokenizer, MySymbol* symbol) {
	return 0;
}
int MyParser::ParseReference(MyTokenizer* tokenizer, MyReference* reference) {
	return 0;
}

int MyParser::ParseAssignList(MyTokenizer* tokenizer, MyAssignmentList* assignList) {
	return 0;
}
int MyParser::ParseAssignment(MyTokenizer* tokenizer, MyAssignment* assignment) {
	return 0;
}

int MyParser::Peek(MyTokenizer* tokenizer) {
	int err = tokenizer->Peek();
	if (err) return LastError(err, tokenizer->LastErrorMessage());
	return 0;
}
int MyParser::Next(MyTokenizer* tokenizer) {
	int err = tokenizer->Next();
	if (err) return LastError(err, tokenizer->LastErrorMessage());
	return 0;
}
int MyParser::ExpectedTokenType(MyTokenizer* tokenizer, int tokType, const char* token) {
	int err = 0;
	if (err = tokenizer->Next()) return LastError(err, tokenizer->LastErrorMessage());
	if (tokenizer->TokenType() != tokType) {
		return LastError(MY_ERR_INVALID_PARAMETERS, "Invalid token"); // TODO: more detail information
	}
	if (token && !tokenizer->Token()->Equals(token)) {
		return LastError(MY_ERR_INVALID_PARAMETERS, "Invalid token"); // TODO: more detail information
	}
	return 0;
}
int MyParser::ExpectedReservedWord(MyTokenizer* tokenizer, const char* token) {
	return ExpectedTokenType(tokenizer, TOKEN_RESERVED_WORD, token);
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

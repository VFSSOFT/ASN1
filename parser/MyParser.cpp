#ifndef _MY_PARSER_CPP_
#define _MY_PARSER_CPP_

#include "MyParser.h"

#include "../../common/MyFile.h"

int MyParser::Parse(const wchar_t* file) {
	int err = 0;

	MyBuffer data;
	MyFile f;
	MyTokenizer tokenizer;
	
	if (err = f.ReadAllBytes(file, &data)) return LastError(err, f.LastErrorMessage());
	tokenizer.Set(data.DerefConst(), data.Length());
	
	do {
		if (err = tokenizer.Next()) 
			return LastError(err, tokenizer.LastErrorMessage());

		m_Tokens.AddNew()->Set(tokenizer.Token());
		m_TokenTypes.Add(tokenizer.TokenType());

	} while (tokenizer.TokenType() != TOKEN_EOF);

	int tokIdx = 0;
	m_ModuleDef = ParseModuleDef(tokIdx);
	if (err != 0) return err;

	return 0;
}

MyModuleDef* MyParser::ParseModuleDef(int& tokIdx) {
	int err = 0;
	MyModuleDef* moduleDef = new MyModuleDef();

	moduleDef->ModuleId = ParseModuleIdentifier(tokIdx);
	if (moduleDef->ModuleId == NULL) return NULL;

	//if (err = ExpectedReservedWord(tokenizer, "DEFINITIONS")) return err;

	//moduleDef->TagDefault = new MyTagDefault();
	//if (err = tokenizer->Peek()) return LastError(err, tokenizer->LastErrorMessage());
	//if (tokenizer->Token()->Equals("EXPLICIT") || tokenizer->Token()->Equals("IMPLICIT") || tokenizer->Token()->Equals("AUTOMATIC")) {
	//	if (err = ParseTagDefaults(tokenizer, moduleDef->TagDefault)) return err;
	//	if (err = tokenizer->Peek()) return LastError(err, tokenizer->LastErrorMessage());
	//} else {
	//	moduleDef->TagDefault->SetValid(true); // Empty
	//}

	//moduleDef->ExtensionDefault = new MyExtensionDefault();
	//if (tokenizer->Token()->Equals("EXTENSIBILITY")) {
	//	moduleDef->ExtensionDefault->Value.Set("EXTENSIBILITY");
	//	if (err = tokenizer->Next()) return LastError(err, tokenizer->LastErrorMessage());
	//	//if (err = tokenizer->Peek()) return LastError(err, tokenizer->LastErrorMessage());
	//}
	//moduleDef->ExtensionDefault->SetValid(true);

	//moduleDef->Body = new MyModuleBody();
	//if (err = ExpectedTokenType(tokenizer, TOKEN_ASSIGNMENT)) return err;
	//if (err = ExpectedReservedWord(tokenizer, "BEGIN")) return err;
	//if (err = ParseModuleBody(tokenizer, moduleDef->Body)) return err;
	//if (err = ExpectedReservedWord(tokenizer, "END")) return err;

	moduleDef->SetValid(true);
	return 0;
}
MyModuleID* MyParser::ParseModuleIdentifier(int& tokIdx) {
	int err = 0;
	MyModuleID* moduleID = new MyModuleID();

	if (err = ExpectedTokenType(tokIdx, TOKEN_TYPE_REF)) return NULL;
	moduleID->ModuleReference.Set(m_Tokens.Get(tokIdx));
	tokIdx++;

	moduleID->DefinitiveID = ParseDefinitiveIdentifier(tokIdx);
	if (moduleID->DefinitiveID == NULL) return NULL;

	return moduleID;
}


MyDefinitiveID* MyParser::ParseDefinitiveIdentifier(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyDefinitiveID* definitiveID = new MyDefinitiveID();

	if (m_TokenTypes.Get(idx) == TOKEN_SINGLE_CHAR_ITEM && m_Tokens.Get(idx)->Equals("{")) {
		definitiveID->Empty = false;
		idx++;

		definitiveID->DefinitiveObjIdComponentList = ParseDefinitiveObjIdComponentList(idx);
		if (definitiveID->DefinitiveObjIdComponentList == NULL) {
			goto done;
		}

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) {
			goto done;
		}

		success = true;
	} else {
		definitiveID->Empty = true;
		success = true;
	}

done:
	if (!success) {
		delete definitiveID;
		return NULL;
	} else {
		tokIdx = idx;
		return definitiveID;
	}
}

MyDefinitiveObjIdComponentList* MyParser::ParseDefinitiveObjIdComponentList(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyDefinitiveObjIdComponentList* list = new MyDefinitiveObjIdComponentList();

	while (true) {
		MyDefinitiveObjIdComponent* comp = ParseDefinitiveObjIdComponent(idx);
		if (comp != NULL) {
			list->DefinitiveObjIdComponents.Add(comp);
			success = true;
		} else {
			break;
		}
	}

done:
	if (!success) {
		delete list;
		return NULL;
	} else {
		tokIdx = idx;
		return list;
	}
}

MyDefinitiveObjIdComponent* MyParser::ParseDefinitiveObjIdComponent(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyDefinitiveObjIdComponent* comp = new MyDefinitiveObjIdComponent();

	comp->DefinitiveNameAndNumberForm = ParseDefinitiveNameAndNumberForm(idx);
	if (comp->DefinitiveNameAndNumberForm == NULL) {
		comp->DefinitiveNumberForm = ParseDefinitiveNumberForm(idx);
		if (comp->DefinitiveNumberForm == NULL) {
			comp->NameForm = ParseNameForm(idx);
			if (comp->NameForm == NULL) {
				goto done;
			}
		}
	}
	success = true;

done:
	if (!success) {
		delete comp;
		return NULL;
	} else {
		tokIdx = idx;
		return comp;
	}
}

MyNameForm* MyParser::ParseNameForm(int& tokIdx) {
	if (m_TokenTypes.Get(tokIdx) == TOKEN_IDENTIFIER) {
		MyNameForm* nameForm = new MyNameForm();
		nameForm->Set(m_Tokens.Get(tokIdx));
		tokIdx++;
		return nameForm;
	}
	return NULL;
}
MyDefinitiveNumberForm* MyParser::ParseDefinitiveNumberForm(int& tokIdx) {
	if (m_TokenTypes.Get(tokIdx) == TOKEN_NUMBER) {
		MyDefinitiveNumberForm* definitiveNumberForm = new MyDefinitiveNumberForm();
		definitiveNumberForm->Number.Set(m_Tokens.Get(tokIdx));
		tokIdx++;
		return definitiveNumberForm;
	}
	return NULL;
}
MyDefinitiveNameAndNumberForm* MyParser::ParseDefinitiveNameAndNumberForm(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyDefinitiveNameAndNumberForm* definitiveNameAndNumberForm = new MyDefinitiveNameAndNumberForm();

	if (m_TokenTypes.Get(idx) == TOKEN_IDENTIFIER) {
		definitiveNameAndNumberForm->Identifier.Set(m_Tokens.Get(idx));
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "(")) {
			goto done;
		}
		idx++;

		definitiveNameAndNumberForm->NumberForm = ParseDefinitiveNumberForm(idx);
		if (definitiveNameAndNumberForm->NumberForm == NULL) {
			goto done;
		}

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ")")) {
			goto done;
		}
		idx++;

		success = true;
	}

done:
	if (!success) {
		delete definitiveNameAndNumberForm;
		return NULL;
	} else {
		tokIdx = idx;
		return definitiveNameAndNumberForm;
	}
}



int MyParser::ExpectedTokenType(int tokIdx, int tokType, const char* token) {
	int err = 0;
	if (m_TokenTypes.Get(tokIdx) != tokType) {
		return LastError(MY_ERR_INVALID_PARAMETERS, "Invalid token"); // TODO: more detail information
	}
	if (token && !m_Tokens.Get(tokIdx)->Equals(token)) {
		return LastError(MY_ERR_INVALID_PARAMETERS, "Invalid token"); // TODO: more detail information
	}
	return 0;
}



#endif // _MY_PARSER_CPP_

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
	bool success = false;
	int idx = tokIdx;
	MyModuleDef* moduleDef = new MyModuleDef();

	moduleDef->ModuleId = ParseModuleIdentifier(idx);
	if (moduleDef->ModuleId == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "DEFINITIONS")) goto done;
	idx++;

	moduleDef->TagDefault = ParseDefaultTag(idx);
	if (moduleDef->TagDefault == NULL) goto done;

	moduleDef->ExtensionDefault = ParseExtensionDefault(idx);
	if (moduleDef->ExtensionDefault == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_ASSIGNMENT)) goto done;
	idx++;
	
	if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "BEGIN")) goto done;
	idx++;

	moduleDef->Body = ParseModuleBody(idx);

	if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "END")) goto done;
	idx++;

	success = true;

done:
	if (!success) {
		delete moduleDef;
		return NULL;
	} else {
		tokIdx = idx;
		return moduleDef;
	}
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
MyTagDefault* MyParser::ParseDefaultTag(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyTagDefault* tagDefault = new MyTagDefault();

	if (m_TokenTypes.Get(idx) == TOKEN_RESERVED_WORD && (m_Tokens.Get(idx)->Equals("IMPLICIT") || m_Tokens.Get(idx)->Equals("AUTOMATIC"))) {
		idx++;

		if (m_TokenTypes.Get(idx) == TOKEN_RESERVED_WORD && m_Tokens.Get(idx)->Equals("TAGS")) {
			idx++;
			success = true;
		}
	} else {
		// empty;
		success = true;
	}

done:
	if (!success) {
		delete tagDefault;
		return NULL;
	} else {
		tokIdx = idx;
		return tagDefault;
	}
}

MyExtensionDefault* MyParser::ParseExtensionDefault(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyExtensionDefault* extDefault = new MyExtensionDefault();

	if (m_TokenTypes.Get(idx) == TOKEN_RESERVED_WORD && m_Tokens.Get(idx)->Equals("EXTENSIBILITY")) {
		idx++;

		if (m_TokenTypes.Get(idx) == TOKEN_RESERVED_WORD && m_Tokens.Get(idx)->Equals("IMPLIED")) {
			idx++;
			success = true;
		}
	} else {
		// empty;
		success = true;
	}

done:
	if (!success) {
		delete extDefault;
		return NULL;
	} else {
		tokIdx = idx;
		return extDefault;
	}
}

MyModuleBody* MyParser::ParseModuleBody(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyModuleBody* body = new MyModuleBody();

	body->Exports = ParseExports(idx);
	if (body->Exports == NULL) goto done;




done:
	if (!success) {
		delete body;
		return NULL;
	} else {
		tokIdx = idx;
		return body;
	}
}

MyExports* MyParser::ParseExports(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyExports* exports = new MyExports();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "EXPORTS")) {
		idx++;

		if (IsToken(idx, TOKEN_RESERVED_WORD, "ALL")) {
			idx++;
			exports->ExportAll = true;
			success = true;
		} else {
			exports->SymbolsExported = ParseSymbolsExported(idx);
			if (exports->SymbolsExported == NULL) goto done;

			if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ";")) goto done;

			success = true;
		}

		success = true;
	} else {
		// empty;
		exports->Empty = true;
		success = true;
	}

done:
	if (!success) {
		delete exports;
		return NULL;
	} else {
		tokIdx = idx;
		return exports;
	}
}

MyImports* MyParser::ParseImports(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyImports* imports = new MyImports();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "IMPORTS")) {
		idx++;

		imports->SymbolsImported = ParseSymbolsImported(idx);
		if (imports->SymbolsImported == NULL) goto done;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ";")) goto done;

		success = true;
	} else {
		imports->Empty = true;
		success = true;
	}

done:
	if (!success) {
		delete imports;
		return NULL;
	} else {
		tokIdx = idx;
		return imports;
	}
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

MySymbolsExported* MyParser::ParseSymbolsExported(int& tokIdx) {
	int idx = tokIdx;
	MySymbolsExported* exported = new MySymbolsExported();

	exported->Symbols = ParseSymbolList(idx);
	if (exported->Symbols == NULL) {
		exported->Empty = true;
	}
	tokIdx = idx;
	return exported;
}
MySymbolList* MyParser::ParseSymbolList(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MySymbolList* list = new MySymbolList();

	while (true) {
		MySymbol* sym = ParseSymbol(idx);
		if (sym) {
			list->Symbols.Add(sym);
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
MySymbol* MyParser::ParseSymbol(int& tokIdx) {
	return 0;
}

MySymbolsImported* MyParser::ParseSymbolsImported(int& tokIdx) {
	int idx = tokIdx;
	MySymbolsImported* imported = new MySymbolsImported();

	imported->List = ParseSymbolsFromModuleList(idx);
	if (imported->List == NULL) {
		imported->Empty = true;
	}
	tokIdx = idx;
	return imported;
}
MySymbolsFromModuleList* MyParser::ParseSymbolsFromModuleList(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MySymbolsFromModuleList* list = new MySymbolsFromModuleList();

	while (true) {
		MySymbolsFromModule* sfm = ParseSymbolsFromModule(idx);
		if (sfm != NULL) {
			list->List.Add(sfm);
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
MySymbolsFromModule* MyParser::ParseSymbolsFromModule(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MySymbolsFromModule* sfm = new MySymbolsFromModule();

	sfm->Symbols = ParseSymbolList(idx);
	if (sfm->Symbols == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "FROM")) goto done;
	idx++;

	sfm->GlobalModuleReference = ParseGlobalModuleReference(idx);
	if (sfm->GlobalModuleReference == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete sfm;
		return NULL;
	} else {
		tokIdx = idx;
		return sfm;
	}
}
MyGlobalModuleReference* MyParser::ParseGlobalModuleReference(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyGlobalModuleReference* ref = new MyGlobalModuleReference();

	if (err = ExpectedTokenType(idx, TOKEN_TYPE_REF)) goto done;
	ref->ModuleReference.Set(m_Tokens.Get(idx));
	idx++;

	ref->AssignedID = ParseAssignedIdentifier(idx);
	if (ref->AssignedID == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete ref;
		return NULL;
	} else {
		tokIdx = idx;
		return ref;
	}
}
MyAssignedID* MyParser::ParseAssignedIdentifier(int& tokIdx) {
	return 0;
}

bool MyParser::IsToken(int tokIdx, int tokType, const char* token) {
	if (m_TokenTypes.Get(tokIdx) != tokType) return false;
	if (token && !m_Tokens.Get(tokIdx)->Equals(token)) return false;
	return true;
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

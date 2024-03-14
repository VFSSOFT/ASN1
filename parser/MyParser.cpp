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

	moduleDef->TagDefault = ParseDefaultTag(idx);
	if (moduleDef->TagDefault == NULL) goto done;

	moduleDef->ExtensionDefault = ParseExtensionDefault(idx);
	if (moduleDef->ExtensionDefault == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_ASSIGNMENT)) goto done;
	
	if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "BEGIN")) goto done;

	moduleDef->Body = ParseModuleBody(idx);

	if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "END")) goto done;

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
	moduleID->ModuleReference.Set(m_Tokens.Get(tokIdx-1));

	moduleID->DefinitiveID = ParseDefinitiveIdentifier(tokIdx);
	if (moduleID->DefinitiveID == NULL) return NULL;

	return moduleID;
}
MyTagDefault* MyParser::ParseDefaultTag(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyTagDefault* tagDefault = new MyTagDefault();

	if (m_TokenTypes.Get(idx) == TOKEN_RESERVED_WORD && (m_Tokens.Get(idx)->Equals("EXPLICIT") || m_Tokens.Get(idx)->Equals("IMPLICIT") || m_Tokens.Get(idx)->Equals("AUTOMATIC"))) {
		tagDefault->Value.Set(m_Tokens.Get(idx));
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

	body->Imports = ParseImports(idx);
	if (body->Exports == NULL) goto done;

	body->AssignmentList = ParseAssignmentList(idx);
	if (body->AssignmentList == NULL) goto done;

	success = true;

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

MyAssignmentList* MyParser::ParseAssignmentList(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyAssignmentList* list = new MyAssignmentList();

	while (true) {
		MyAssignment* ass = ParseAssignment(idx);
		if (ass != NULL) {
			list->List.Add(ass);
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

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

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

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "(")) goto done;

		definitiveNameAndNumberForm->NumberForm = ParseDefinitiveNumberForm(idx);
		if (definitiveNameAndNumberForm->NumberForm == NULL) {
			goto done;
		}

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ")")) goto done;

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
	ref->ModuleReference.Set(m_Tokens.Get(idx-1));

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
	assert(false);
	return 0;
}
MyObjectIDValue* MyParser::ParseObjectIDValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjectIDValue* val = new MyObjectIDValue();

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) goto done;

	val->DefinedValue = ParseDefinedValue(idx);
	// No matter whether it's successful or not

	val->ObjIdComponentsList = ParseObjIdComponentsList(idx);
	if (val->ObjIdComponentsList == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

	success = true;

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MyObjIdComponentsList* MyParser::ParseObjIdComponentsList(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyObjIdComponentsList* list = new MyObjIdComponentsList();

	while (true) {
		MyObjIdComponents* comps = ParseObjIdComponents(idx);
		if (comps != NULL) {
			list->ObjIdComponents.Add(comps);
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
MyObjIdComponents* MyParser::ParseObjIdComponents(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjIdComponents* comps = new MyObjIdComponents();

	comps->NameAndNumberForm = ParseNameAndNumberForm(idx);
	if (comps->NameAndNumberForm) { success = true; goto done; }

	comps->NumberForm = ParseNumberForm(idx);
	if (comps->NumberForm) { success = true; goto done; }

	comps->DefinedValue = ParseDefinedValue(idx);
	if (comps->DefinedValue) { success = true; goto done; }

	comps->NameForm = ParseNameForm(idx);
	if (comps->NameForm) { success = true; goto done; }

done:
	if (!success) {
		delete comps;
		return NULL;
	} else {
		tokIdx = idx;
		return comps;
	}
}
MyNameAndNumberForm* MyParser::ParseNameAndNumberForm(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyNameAndNumberForm* form = new MyNameAndNumberForm();

	if (IsToken(idx, TOKEN_IDENTIFIER)) {
		form->Identifier.Set(m_Tokens.Get(idx));
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "(")) goto done;

		form->NumberForm = ParseNumberForm(idx);
		if (form->NumberForm == NULL) goto done;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ")")) goto done;

		success = true;
	}

done:
	if (!success) {
		delete form;
		return NULL;
	} else {
		tokIdx = idx;
		return form;
	}
}
MyNumberForm* MyParser::ParseNumberForm(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyNumberForm* numberForm = new MyNumberForm();

	if (IsToken(idx, TOKEN_NUMBER)) {
		numberForm->Number.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
		goto done;
	}

	numberForm->DefinedValue = ParseDefinedValue(idx);
	if (numberForm->DefinedValue == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete numberForm;
		return NULL;
	} else {
		tokIdx = idx;
		return numberForm;
	}
}

MyAssignment* MyParser::ParseAssignment(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyAssignment* ass = new MyAssignment();

	ass->TypeAssignment = ParseTypeAssignment(idx);
	if (ass->TypeAssignment) { success = true; goto done; }

	ass->ValueAssignment = ParseValueAssignment(idx);
	if (ass->ValueAssignment) { success = true; goto done; }

	ass->ValueSetTypeAssignment = ParseValueSetTypeAssignment(idx);
	if (ass->ValueSetTypeAssignment) { success = true; goto done; }

	ass->ObjectClassAssignment = ParseObjectClassAssignment(idx);
	if (ass->ObjectClassAssignment) { success = true; goto done; }

	ass->ObjectAssignment = ParseObjectAssignment(idx);
	if (ass->ObjectAssignment) { success = true; goto done; }

	ass->ObjectSetAssignment = ParseObjectSetAssignment(idx);
	if (ass->ObjectSetAssignment) { success = true; goto done; }

	ass->ParameterizedAssignment = ParseParameterizedAssignment(idx);
	if (ass->ParameterizedAssignment) { success = true; goto done; }

done:
	if (!success) {
		delete ass;
		return NULL;
	} else {
		tokIdx = idx;
		return ass;
	}
}
MyTypeAssignment* MyParser::ParseTypeAssignment(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyTypeAssignment* ass = new MyTypeAssignment();

	if (IsToken(idx, TOKEN_TYPE_REF)) {
		ass->TypeReference.Set(m_Tokens.Get(idx));
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_ASSIGNMENT, "::=")) goto done;

		ass->Type = ParseType(idx);
		if (ass->Type == NULL) goto done;

		success = true;
	}

done:
	if (!success) {
		delete ass;
		return NULL;
	} else {
		tokIdx = idx;
		return ass;
	}
}
MyValueAssignment* MyParser::ParseValueAssignment(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyValueAssignment* ass = new MyValueAssignment();

	if (IsToken(idx, TOKEN_IDENTIFIER)) {
		ass->ValueReference.Set(m_Tokens.Get(idx));
		idx++;

		ass->Type = ParseType(idx);
		if (ass->Type == NULL) goto done;

		if (err = ExpectedTokenType(idx, TOKEN_ASSIGNMENT, "::=")) goto done;

		ass->Value = ParseValue(idx);
		if (ass->Value == NULL) goto done;

		success = true;
	}

done:
	if (!success) {
		delete ass;
		return NULL;
	} else {
		tokIdx = idx;
		return ass;
	}
}
MyValueSetTypeAssignment* MyParser::ParseValueSetTypeAssignment(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyValueSetTypeAssignment* ass = new MyValueSetTypeAssignment();

	if (IsToken(idx, TOKEN_TYPE_REF)) {
		ass->TypeReference.Set(m_Tokens.Get(idx));
		idx++;

		ass->Type = ParseType(idx);
		if (ass->Type == NULL) goto done;

		if (err = ExpectedTokenType(idx, TOKEN_ASSIGNMENT, "::=")) goto done;

		ass->ValueSet = ParseValueSet(idx);
		if (ass->ValueSet == NULL) goto done;

		success = true;
	}

done:
	if (!success) {
		delete ass;
		return NULL;
	} else {
		tokIdx = idx;
		return ass;
	}
}
MyObjectClassAssignment* MyParser::ParseObjectClassAssignment(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjectClassAssignment* ass = new MyObjectClassAssignment();

	if (IsToken(idx, TOKEN_TYPE_REF)) {
		ass->ObjectClassReference.Set(m_Tokens.Get(idx));
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_ASSIGNMENT, "::=")) goto done;

		ass->ObjectClass = ParseObjectClass(idx);
		if (ass->ObjectClass == NULL) goto done;

		success = true;
	}

done:
	if (!success) {
		delete ass;
		return NULL;
	} else {
		tokIdx = idx;
		return ass;
	}
}
MyObjectAssignment* MyParser::ParseObjectAssignment(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjectAssignment* ass = new MyObjectAssignment();

	if (IsToken(idx, TOKEN_VALUE_REF)) {
		ass->ObjectReference.Set(m_Tokens.Get(idx));
		idx++;

		ass->DefinedObjectClass = ParseDefinedObjectClass(idx);
		if (ass->DefinedObjectClass == NULL) goto done;

		if (err = ExpectedTokenType(idx, TOKEN_ASSIGNMENT, "::=")) goto done;

		ass->Object = ParseObject(idx);
		if (ass->Object == NULL) goto done;

		success = true;
	}

done:
	if (!success) {
		delete ass;
		return NULL;
	} else {
		tokIdx = idx;
		return ass;
	}
}
MyObjectSetAssignment* MyParser::ParseObjectSetAssignment(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjectSetAssignment* ass = new MyObjectSetAssignment();

	if (IsToken(idx, TOKEN_TYPE_REF)) {
		ass->ObjectSetReference.Set(m_Tokens.Get(idx));
		idx++;

		ass->DefinedObjectClass = ParseDefinedObjectClass(idx);
		if (ass->DefinedObjectClass == NULL) goto done;

		if (err = ExpectedTokenType(idx, TOKEN_ASSIGNMENT, "::=")) goto done;

		ass->ObjectSet = ParseObjectSet(idx);
		if (ass->ObjectSet == NULL) goto done;

		success = true;
	}

done:
	if (!success) {
		delete ass;
		return NULL;
	} else {
		tokIdx = idx;
		return ass;
	}
}
MyParameterizedAssignment* MyParser::ParseParameterizedAssignment(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyParameterizedAssignment* ass = new MyParameterizedAssignment();

	ass->ParameterizedTypeAssignment = ParseParameterizedTypeAssignment(idx);
	if (ass->ParameterizedTypeAssignment) { success = true; goto done; }

	ass->ParameterizedValueAssignment = ParseParameterizedValueAssignment(idx);
	if (ass->ParameterizedValueAssignment) { success = true; goto done; }

	ass->ParameterizedValueSetTypeAssignment = ParseParameterizedValueSetTypeAssignment(idx);
	if (ass->ParameterizedValueSetTypeAssignment) { success = true; goto done; }

	ass->ParameterizedObjectClassAssignment = ParseParameterizedObjectClassAssignment(idx);
	if (ass->ParameterizedObjectClassAssignment) { success = true; goto done; }

	ass->ParameterizedObjectAssignment = ParseParameterizedObjectAssignment(idx);
	if (ass->ParameterizedObjectAssignment) { success = true; goto done; }

	ass->ParameterizedObjectSetAssignment = ParseParameterizedObjectSetAssignment(idx);
	if (ass->ParameterizedObjectSetAssignment) { success = true; goto done; }

done:
	if (!success) {
		delete ass;
		return NULL;
	} else {
		tokIdx = idx;
		return ass;
	}
}
MyParameterizedTypeAssignment* MyParser::ParseParameterizedTypeAssignment(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyParameterizedTypeAssignment* ass = new MyParameterizedTypeAssignment();

	if (err = ExpectedTokenType(idx, TOKEN_TYPE_REF)) goto done;
	ass->TypeReference.Set(m_Tokens.Get(idx-1));

	ass->ParameterList = ParseParameterList(idx);
	if (ass->ParameterList == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_ASSIGNMENT, "::=")) goto done;

	ass->Type = ParseType(idx);
	if (ass->Type == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete ass;
		return NULL;
	} else {
		tokIdx = idx;
		return ass;
	}
}
MyParameterizedValueAssignment* MyParser::ParseParameterizedValueAssignment(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyParameterizedValueAssignment* ass = new MyParameterizedValueAssignment();

	if (err = ExpectedTokenType(idx, TOKEN_VALUE_REF)) goto done;
	ass->ValueReference.Set(m_Tokens.Get(idx-1));

	ass->ParameterList = ParseParameterList(idx);
	if (ass->ParameterList == NULL) goto done;

	ass->Type = ParseType(idx);
	if (ass->Type == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_ASSIGNMENT, "::=")) goto done;

	ass->Value = ParseValue(idx);
	if (ass->Value == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete ass;
		return NULL;
	} else {
		tokIdx = idx;
		return ass;
	}
}
MyParameterizedValueSetTypeAssignment* MyParser::ParseParameterizedValueSetTypeAssignment(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyParameterizedValueSetTypeAssignment* ass = new MyParameterizedValueSetTypeAssignment();

	if (err = ExpectedTokenType(idx, TOKEN_TYPE_REF)) goto done;
	ass->TypeReference.Set(m_Tokens.Get(idx-1));

	ass->ParameterList = ParseParameterList(idx);
	if (ass->ParameterList == NULL) goto done;

	ass->Type = ParseType(idx);
	if (ass->Type == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_ASSIGNMENT, "::=")) goto done;

	ass->ValueSet = ParseValueSet(idx);
	if (ass->ValueSet == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete ass;
		return NULL;
	} else {
		tokIdx = idx;
		return ass;
	}
}
MyParameterizedObjectClassAssignment* MyParser::ParseParameterizedObjectClassAssignment(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyParameterizedObjectClassAssignment* ass = new MyParameterizedObjectClassAssignment();

	if (!ParseObjectClassReference(idx, &ass->ObjectClassReference)) goto done;

	ass->ParameterList = ParseParameterList(idx);
	if (ass->ParameterList == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_ASSIGNMENT, "::=")) goto done;

	ass->ObjectClass = ParseObjectClass(idx);
	if (ass->ObjectClass == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete ass;
		return NULL;
	} else {
		tokIdx = idx;
		return ass;
	}
}
MyParameterizedObjectAssignment* MyParser::ParseParameterizedObjectAssignment(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyParameterizedObjectAssignment* ass = new MyParameterizedObjectAssignment();

	if (err = ExpectedTokenType(idx, TOKEN_VALUE_REF)) goto done;
	ass->ObjectReference.Set(m_Tokens.Get(idx-1));

	ass->ParameterList = ParseParameterList(idx);
	if (ass->ParameterList == NULL) goto done;

	ass->DefinedObjectClass = ParseDefinedObjectClass(idx);
	if (ass->DefinedObjectClass == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_ASSIGNMENT, "::=")) goto done;

	ass->Object = ParseObject(idx);
	if (ass->Object == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete ass;
		return NULL;
	} else {
		tokIdx = idx;
		return ass;
	}
}
MyParameterizedObjectSetAssignment* MyParser::ParseParameterizedObjectSetAssignment(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyParameterizedObjectSetAssignment* ass = new MyParameterizedObjectSetAssignment();

	if (err = ExpectedTokenType(idx, TOKEN_TYPE_REF)) goto done;
	ass->ObjectSetReference.Set(m_Tokens.Get(idx-1));

	ass->ParameterList = ParseParameterList(idx);
	if (ass->ParameterList == NULL) goto done;

	ass->DefinedObjectClass = ParseDefinedObjectClass(idx);
	if (ass->DefinedObjectClass == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_ASSIGNMENT, "::=")) goto done;

	ass->ObjectSet = ParseObjectSet(idx);
	if (ass->ObjectSet == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete ass;
		return NULL;
	} else {
		tokIdx = idx;
		return ass;
	}
}

MyType* MyParser::ParseType(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyType* typ = new MyType();

	typ->BuiltinType = ParseBuiltinType(idx);
	if (typ->BuiltinType != NULL) { success = true; goto done; }

	typ->ReferencedType = ParseReferencedType(idx);
	if (typ->ReferencedType) { success = true; goto done; }

	typ->ConstrainedType = ParseConstrainedType(idx);
	if (typ->ConstrainedType) { success = true; goto done; }

done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}

MyBuiltinType* MyParser::ParseBuiltinType(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyBuiltinType* typ = new MyBuiltinType();

	typ->BitStringType = ParseBitStringType(idx);
	if (typ->BitStringType) { success = true; goto done; }

	typ->BooleanType = ParseBoolType(idx);
	if (typ->BooleanType) { success = true; goto done; }

	typ->ChoiceType = ParseChoiceType(idx);
	if (typ->ChoiceType) { success = true; goto done; }

	typ->EnumeratedType = ParseEnumeratedType(idx);
	if (typ->EnumeratedType) { success = true; goto done; }

	typ->ExternalType = ParseExternalType(idx);
	if (typ->ExternalType) { success = true; goto done; }

	typ->IntegerType = ParseIntegerType(idx);
	if (typ->IntegerType) { success = true; goto done; }

	typ->NullType = ParseNullType(idx);
	if (typ->NullType) { success = true; goto done; }

	typ->ObjectClassFieldType = ParseObjectClassFieldType(idx);
	if (typ->ObjectClassFieldType) { success = true; goto done; }

	typ->ObjectIDType = ParseObjectIDType(idx);
	if (typ->ObjectIDType) { success = true; goto done; }

	typ->OctetStringType = ParseOctetStringType(idx);
	if (typ->OctetStringType) { success = true; goto done; }

	typ->RealType = ParseRealType(idx);
	if (typ->RealType) { success = true; goto done; }

	typ->SequenceType = ParseSequenceType(idx);
	if (typ->SequenceType) { success = true; goto done; }

	typ->SequenceOfType = ParseSequenceOfType(idx);
	if (typ->SequenceOfType) { success = true; goto done; }

	typ->SetType = ParseSetType(idx);
	if (typ->SetType) { success = true; goto done; }

	typ->SetOfType = ParseSetOfType(idx);
	if (typ->SetOfType) { success = true; goto done; }

	typ->TaggedType = ParseTaggedType(idx);
	if (typ->TaggedType) { success = true; goto done; }

done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MyBitStringType* MyParser::ParseBitStringType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyBitStringType* typ = new MyBitStringType();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "BIT")) {
		idx++;

		if (IsToken(idx, TOKEN_RESERVED_WORD, "STRING")) {
			idx++;

			if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) {
				idx++;

				typ->NamedBitList = ParseNamedBitList(idx);
				if (typ->NamedBitList == NULL) goto done;

				if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;
			}

			success = true;
		}
	}

done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}

}
MyNamedBitList* MyParser::ParseNamedBitList(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyNamedBitList* list = new MyNamedBitList();

	while (true) {
		MyNamedBit* bit = ParseNamedBit(idx);
		if (bit != NULL) {
			list->NamedBits.Add(bit);
			success = true;

			if (!IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) break;
			idx++;
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
MyNamedBit* MyParser::ParseNamedBit(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyNamedBit* bit = new MyNamedBit();

	if (IsToken(idx, TOKEN_IDENTIFIER)) {
		bit->Identifier.Set(m_Tokens.Get(idx));
		idx++;

		if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "(")) {

			bit->DefinedValue = ParseDefinedValue(idx);
			if (bit->DefinedValue == NULL) {

				if (IsToken(idx, TOKEN_NUMBER)) {
					bit->Number.Set(m_Tokens.Get(idx));
					idx++;
				} else {
					goto done; // failed
				}

			}

			if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ")")) goto done;

			success = true;
		}
	}
	
done:
	if (!success) {
		delete bit;
		return NULL;
	} else {
		tokIdx = idx;
		return bit;
	}
}
MyBooleanType* MyParser::ParseBoolType(int& tokIdx) {
	if (IsToken(tokIdx, TOKEN_RESERVED_WORD, "BOOLEAN")) {
		tokIdx++;
		return new MyBooleanType();
	}
	return NULL;
}
MyChoiceType* MyParser::ParseChoiceType(int& tokIdx) {
  int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyChoiceType* typ = new MyChoiceType();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "CHOICE")) {
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) {

			typ->AlternativeTypeLists = ParseAlternativeTypeLists(idx);
			if (typ->AlternativeTypeLists == NULL) goto done;

			if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;
			success = true;
		}
	}
	
done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MyEnumeratedType* MyParser::ParseEnumeratedType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyEnumeratedType* typ = new MyEnumeratedType();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "ENUMERATED")) {
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) goto done;

		typ->Enumerations = ParseEnumerations(idx);

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) goto done;

		success = true;
	}
	
done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MyExternalType* MyParser::ParseExternalType(int& tokIdx) {
	if (IsToken(tokIdx, TOKEN_RESERVED_WORD, "EXTERNAL")) {
		tokIdx++;
		return new MyExternalType();
	}
	return NULL;
}
MyIntegerType* MyParser::ParseIntegerType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyIntegerType* typ = new MyIntegerType();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "INTEGER")) {
		idx++;

		if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) {
			idx++;

			typ->NamedNumberList = ParseNamedNumberList(idx);
			if (typ->NamedNumberList == NULL) goto done;

			if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;
		}

		success = true;
	}
	
done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MyNullType* MyParser::ParseNullType(int& tokIdx) {
	if (IsToken(tokIdx, TOKEN_RESERVED_WORD, "NULL")) {
		tokIdx++;
		return new MyNullType();
	}
	return NULL;
}
MyObjectClassFieldType* MyParser::ParseObjectClassFieldType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjectClassFieldType* fieldType = new MyObjectClassFieldType();

	fieldType->DefinedObjectClass = ParseDefinedObjectClass(idx);
	if (fieldType->DefinedObjectClass == NULL) { goto done; }

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ".")) goto done;

	fieldType->FieldName = ParseFieldName(idx);
	if (fieldType->FieldName == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete fieldType;
		return NULL;
	} else {
		tokIdx = idx;
		return fieldType;
	}
}
MyObjectIDType* MyParser::ParseObjectIDType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjectIDType* objIdType = new MyObjectIDType();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "OBJECT")) {
		idx++;

		if (IsToken(idx, TOKEN_RESERVED_WORD, "IDENTIFIER")) {
			idx++;
			success = true;
		}
	}

done:
	if (!success) {
		delete objIdType;
		return NULL;
	} else {
		tokIdx = idx;
		return objIdType;
	}
}
MyOctetStringType* MyParser::ParseOctetStringType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyOctetStringType* typ = new MyOctetStringType();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "OCTET")) {
		idx++;

		if (IsToken(idx, TOKEN_RESERVED_WORD, "STRING")) {
			idx++;
			success = true;
		}
	}

done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MyRealType* MyParser::ParseRealType(int& tokIdx) {
	if (IsToken(tokIdx, TOKEN_RESERVED_WORD, "REAL")) {
		tokIdx++;
		return new MyRealType();
	}
	return NULL;
}
MySequenceType* MyParser::ParseSequenceType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MySequenceType* typ = new MySequenceType();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "SEQUENCE")) {
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) goto done;

		if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) {
			idx++;
			success = true;
			goto done;
		}

		typ->ComponentTypeLists = ParseComponentTypeLists(idx);
		if (typ->ComponentTypeLists) {
			if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

			success = true;
			goto done;
		}

		typ->ExtensionAndException = ParseExtensionAndException(idx);
		if (typ->ExtensionAndException == NULL) goto done;

		typ->OptionalExtensionMarker = ParseOptionalExtensionMarker(idx);
		if (typ->OptionalExtensionMarker == NULL) goto done;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;
		success = true;
	}
	
done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MySequenceOfType* MyParser::ParseSequenceOfType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MySequenceOfType* typ = new MySequenceOfType();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "SEQUENCE")) {
		idx++;

		if (IsToken(idx, TOKEN_RESERVED_WORD, "OF")) {
			idx++;

			typ->Type = ParseType(idx);
			if (typ->Type) { success = true; goto done; }

			typ->NamedType = ParseNamedType(idx);
			if (typ->NamedType) { success = true; goto done; }
		}
	}
	
done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MySetType* MyParser::ParseSetType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MySetType* typ = new MySetType();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "SET")) {
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) goto done;
		if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) {
			idx++;
			success = true;
			goto done;
		}

		typ->ComponentTypeLists = ParseComponentTypeLists(idx);
		if (typ->ComponentTypeLists) {
			if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;
			success = true;
			goto done;
		}

		typ->ExtensionAndException = ParseExtensionAndException(idx);
		if (typ->ExtensionAndException == NULL) goto done;

		typ->OptionalExtensionMarker = ParseOptionalExtensionMarker(idx);
		if (typ->OptionalExtensionMarker == NULL) goto done;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;
		success = true;
	}
	
done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MySetOfType* MyParser::ParseSetOfType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MySetOfType* typ = new MySetOfType();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "SET")) {
		idx++;

		if (IsToken(idx, TOKEN_RESERVED_WORD, "OF")) {
			idx++;

			typ->Type = ParseType(idx);
			if (typ->Type) { success = true; goto done; }

			typ->NamedType = ParseNamedType(idx);
			if (typ->NamedType) { success = true; goto done; }
		}
	}
	
done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MyTaggedType* MyParser::ParseTaggedType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyTaggedType* typ = new MyTaggedType();

	typ->Tag = ParseTag(idx);
	if (typ->Tag == NULL) goto done;

	if (IsToken(idx, TOKEN_RESERVED_WORD, "IMPLICIT") || IsToken(idx, TOKEN_RESERVED_WORD, "EXPLICIT")) {
		typ->ImplicitOrExplicit.Set(m_Tokens.Get(idx));
		idx++;
	}

	typ->Type = ParseType(idx);
	if (typ->Type == NULL) goto done;

	success = true;
	
done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MyReferencedType* MyParser::ParseReferencedType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyReferencedType* typ = new MyReferencedType();

	typ->DefinedType = ParseDefinedType(idx);
	if (typ->DefinedType) { success = true; goto done; }

	if (IsToken(idx, TOKEN_TYPE_REF)) {
		typ->UsefulType.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
		goto done;
	}

	typ->SelectionType = ParseSelectionType(idx);
	if (typ->SelectionType) { success = true; goto done; }

	typ->TypeFromObject = ParseTypeFromObject(idx);
	if (typ->TypeFromObject) { success = true; goto done; }

	typ->ValueSetFromObjects = ParseValueSetFromObjects(idx);
	if (typ->ValueSetFromObjects) { success = true; goto done; }

done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MyDefinedType* MyParser::ParseDefinedType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyDefinedType* typ = new MyDefinedType();

	typ->ExternalTypeReference = ParseExternalTypeReference(idx);
	if (typ->ExternalTypeReference) { success = true; goto done; }

	typ->ParameterizedType = ParseParameterizedType(idx);
	if (typ->ParameterizedType) { success = true; goto done; }

	typ->ParameterizedValueSetType = ParseParameterizedValueSetType(idx);
	if (typ->ParameterizedValueSetType) { success = true; goto done; }

	if (IsToken(idx, TOKEN_TYPE_REF)) {
		typ->TypeReference.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
	}

done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MyExternalTypeReference* MyParser::ParseExternalTypeReference(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyExternalTypeReference* ref = new MyExternalTypeReference();

	if (IsToken(idx, TOKEN_MODULE_REF)) {
		ref->ModuleReference.Set(m_Tokens.Get(idx));
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ".")) goto done;

		if (IsToken(idx, TOKEN_TYPE_REF)) {
			ref->TypeReference.Set(m_Tokens.Get(idx));
			idx++;
			success = true;
		}
	}

done:
	if (!success) {
		delete ref;
		return NULL;
	} else {
		tokIdx = idx;
		return ref;
	}
}
MyParameterizedType* MyParser::ParseParameterizedType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyParameterizedType* typ = new MyParameterizedType();

	typ->SimpleDefinedType = ParseSimpleDefinedType(idx);
	if (typ->SimpleDefinedType == NULL) goto done;

	typ->ActualParameterList = ParseActualParameterList(idx);
	if (typ->ActualParameterList == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MySimpleDefinedType* MyParser::ParseSimpleDefinedType(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MySimpleDefinedType* typ = new MySimpleDefinedType();

	typ->ExternalTypeReference = ParseExternalTypeReference(idx);
	if (typ->ExternalTypeReference) { success = true; goto done; }

	if (IsToken(idx, TOKEN_TYPE_REF)) {
		typ->TypeReference.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
	}

done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MyActualParameterList* MyParser::ParseActualParameterList(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyActualParameterList* list = new MyActualParameterList();

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) {
		idx++;

		while (true) {
			MyActualParameter* p = ParseActualParameter(idx);
			if (p != NULL) {
				list->ActualParameters.Add(p);

				if (!IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) break;
				idx++;
			} else {
				break;
			}
		}

		if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) {
			idx++;
			success = true;
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
MyActualParameter* MyParser::ParseActualParameter(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyActualParameter* ap = new MyActualParameter();

	ap->Type = ParseType(idx);
	if (ap->Type) { success = true; goto done; }

	ap->Value = ParseValue(idx);
	if (ap->Value) { success = true; goto done; }

	ap->ValueSet = ParseValueSet(idx);
	if (ap->ValueSet) { success =true; goto done; }

	ap->DefinedObjectClass = ParseDefinedObjectClass(idx);
	if (ap->DefinedObjectClass) { success = true; goto done; }

	ap->Object = ParseObject(idx);
	if (ap->Object) { success = true; goto done; }

	ap->ObjectSet = ParseObjectSet(idx);
	if (ap->ObjectSet) { success = true; goto done; }

done:
	if (!success) {
		delete ap;
		return NULL;
	} else {
		tokIdx = idx;
		return ap;
	}
}
MyParameterizedValueSetType* MyParser::ParseParameterizedValueSetType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyParameterizedValueSetType* typ = new MyParameterizedValueSetType();

	typ->SimpleDefinedType = ParseSimpleDefinedType(idx);
	if (typ->SimpleDefinedType == NULL) goto done;

	typ->ActualParameterList = ParseActualParameterList(idx);
	if (typ->ActualParameterList == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MySelectionType* MyParser::ParseSelectionType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MySelectionType* typ = new MySelectionType();

	if (IsToken(idx, TOKEN_IDENTIFIER)) {
		typ->Identifier.Set(m_Tokens.Get(idx));
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "<")) goto done;

		typ->Type = ParseType(idx);
		if (typ->Type == NULL) goto done;

		success = true;
	}

	success = true;

done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MyTypeFromObject* MyParser::ParseTypeFromObject(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyTypeFromObject* typFromObj = new MyTypeFromObject();

	typFromObj->ReferencedObjects = ParseReferencedObjects(idx);
	if (typFromObj->ReferencedObjects == NULL) goto done;

	if (err =ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ".")) goto done;

	typFromObj->FieldName = ParseFieldName(idx);
	if (typFromObj->FieldName == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete typFromObj;
		return NULL;
	} else {
		tokIdx = idx;
		return typFromObj;
	}
}
MyReferencedObjects* MyParser::ParseReferencedObjects(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyReferencedObjects* objs = new MyReferencedObjects();

	objs->DefinedObject = ParseDefinedObject(idx);
	if (objs->DefinedObject) { success = true; goto done; }

	objs->ParameterizedObject = ParseParameterizedObject(idx);
	if (objs->ParameterizedObject) { success = true; goto done; }

	objs->DefinedObjectSet = ParseDefinedObjectSet(idx);
	if (objs->DefinedObjectSet) { success = true; goto done; }

	objs->ParameterizedObjectSet = ParseParameterizedObjectSet(idx);
	if (objs->ParameterizedObjectSet) { success = true; goto done; }

done:
	if (!success) {
		delete objs;
		return NULL;
	} else {
		tokIdx = idx;
		return objs;
	}
}
MyValueSetFromObjects* MyParser::ParseValueSetFromObjects(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyValueSetFromObjects* objs = new MyValueSetFromObjects();

	objs->ReferencedObjects = ParseReferencedObjects(idx);
	if (objs->ReferencedObjects == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ".")) goto done;

	objs->FieldName = ParseFieldName(idx);
	if (objs->FieldName == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete objs;
		return NULL;
	} else {
		tokIdx = idx;
		return objs;
	}
}


MyConstrainedType* MyParser::ParseConstrainedType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyConstrainedType* typ = new MyConstrainedType();

	typ->Type = ParseType(idx);
	if (typ->Type) {
		typ->Constraint = ParseConstraint(idx);
		if (typ->Constraint) {
			success = true;
			goto done;
		}
	}

	typ->TypeWithConstraint = ParseTypeWithConstraint(idx);
	if (typ->TypeWithConstraint == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MyConstraint* MyParser::ParseConstraint(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyConstraint* c = new MyConstraint();

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "(")) goto done;

	c->ConstraintSpec = ParseConstraintSpec(idx);
	if (c->ConstraintSpec == NULL) goto done;

	c->ExceptionSpec = ParseExceptionSpec(idx);
	if (c->ExceptionSpec == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ")")) goto done;
	success = true;

done:
	if (!success) {
		delete c;
		return NULL;
	} else {
		tokIdx = idx;
		return c;
	}
}
MyTypeWithConstraint* MyParser::ParseTypeWithConstraint(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyTypeWithConstraint* c = new MyTypeWithConstraint();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "SET")) {
		idx++;

		c->Constraint = ParseConstraint(idx);
		if (c->Constraint) {
			if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "OF")) goto done;

			c->Type = ParseType(idx);
			if (c->Type) { success = true; goto done; }

			c->NamedType = ParseNamedType(idx);
			if (c->NamedType) { success = true; goto done;}
		} else {
			c->SizeConstraint = ParseSizeConstraint(idx);
			if (c->SizeConstraint == NULL) goto done;

			if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "OF")) goto done;

			c->Type = ParseType(idx);
			if (c->Type) { success = true; goto done; }

			c->NamedType = ParseNamedType(idx);
			if (c->NamedType) { success = true; goto done;}
		}
	}

	if (IsToken(idx, TOKEN_RESERVED_WORD, "SEQUENCE")) {
		idx++;

		c->Constraint = ParseConstraint(idx);
		if (c->Constraint) {
			if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "OF")) goto done;

			c->Type = ParseType(idx);
			if (c->Type) { success = true; goto done; }

			c->NamedType = ParseNamedType(idx);
			if (c->NamedType) { success = true; goto done;}
		} else {
			c->SizeConstraint = ParseSizeConstraint(idx);
			if (c->SizeConstraint == NULL) goto done;

			if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "OF")) goto done;

			c->Type = ParseType(idx);
			if (c->Type) { success = true; goto done; }

			c->NamedType = ParseNamedType(idx);
			if (c->NamedType) { success = true; goto done;}
		}
	}

done:
	if (!success) {
		delete c;
		return NULL;
	} else {
		tokIdx = idx;
		return c;
	}
}
MyIntersectionMark* MyParser::ParseIntersectionMark(int& tokIdx) {
	if (IsToken(tokIdx, TOKEN_SINGLE_CHAR_ITEM, "^")) {
		MyIntersectionMark* mark = new MyIntersectionMark();
		mark->Value.Set("^");
		tokIdx++;
		return mark;
	}
	if (IsToken(tokIdx, TOKEN_RESERVED_WORD, "INTERSECTION")) {
		MyIntersectionMark* mark = new MyIntersectionMark();
		mark->Value.Set("INTERSECTION");
		tokIdx++;
		return mark;
	}
	return NULL;
}
MyUnionMark* MyParser::ParseUnionMark(int& tokIdx) {
	if (IsToken(tokIdx, TOKEN_SINGLE_CHAR_ITEM, "|")) {
		MyUnionMark* mark = new MyUnionMark();
		mark->Value.Set("|");
		tokIdx++;
		return mark;
	}
	if (IsToken(tokIdx, TOKEN_RESERVED_WORD, "UNION")) {
		MyUnionMark* mark = new MyUnionMark();
		mark->Value.Set("UNION");
		tokIdx++;
		return mark;
	}
	return NULL;
}
MyIncludes* MyParser::ParseIncludes(int& tokIdx) {
	MyIncludes* includes = new MyIncludes();
	if (IsToken(tokIdx, TOKEN_RESERVED_WORD, "INCLUDES")) {
		includes->Value.Set("INCLUDES");
		tokIdx++;
	}
	return includes;
}
MyContainedSubtype* MyParser::ParseContainedSubtype(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyContainedSubtype* subtype = new MyContainedSubtype();

	subtype->Includes = ParseIncludes(idx);
	if (subtype->Includes == NULL) goto done;

	subtype->Type = ParseType(idx);
	if (subtype->Type == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete subtype;
		return NULL;
	} else {
		tokIdx = idx;
		return subtype;
	}
}
MyLowerEndValue* MyParser::ParseLowerEndValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyLowerEndValue* lev = new MyLowerEndValue();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "MIN")) {
		idx++;
		lev->MIN = true;
		success = true;
		goto done;
	}

	lev->Value = ParseValue(idx);
	if (lev->Value == NULL) goto done;

	success = true;
	
done:
	if (!success) {
		delete lev;
		return NULL;
	} else {
		tokIdx = idx;
		return lev;
	}
}
MyUpperEndValue* MyParser::ParseUpperEndValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyUpperEndValue* uev = new MyUpperEndValue();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "MAX")) {
		idx++;
		uev->MAX = true;
		success = true;
		goto done;
	}

	uev->Value = ParseValue(idx);
	if (uev->Value == NULL) goto done;

	success = true;
	
done:
	if (!success) {
		delete uev;
		return NULL;
	} else {
		tokIdx = idx;
		return uev;
	}
}
MyLowerEndpoint* MyParser::ParseLowerEndpoint(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyLowerEndpoint* lep = new MyLowerEndpoint();

	lep->LowerEndValue = ParseLowerEndValue(idx);
	if (lep->LowerEndValue == NULL) goto done;

	if (IsToken(idx, TOKEN_RESERVED_WORD, "<")) {
		idx++;
		lep->LessThan = true;
	}

	success = true;
	
done:
	if (!success) {
		delete lep;
		return NULL;
	} else {
		tokIdx = idx;
		return lep;
	}
}
MyUpperEndpoint* MyParser::ParseUpperEndpoint(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyUpperEndpoint* uep = new MyUpperEndpoint();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "<")) {
		idx++;
		uep->LessThan = true;
	}

	uep->UpperEndValue = ParseUpperEndValue(idx);
	if (uep->UpperEndValue == NULL) goto done;

	success = true;
	
done:
	if (!success) {
		delete uep;
		return NULL;
	} else {
		tokIdx = idx;
		return uep;
	}
}
MyValueRange* MyParser::ParseValueRange(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyValueRange* range = new MyValueRange();

	range->LowerEndpoint = ParseLowerEndpoint(idx);
	if (range->LowerEndpoint == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_RANGE_SEPARATOR, "..")) goto done;

	range->UpperEndpoint = ParseUpperEndpoint(idx);
	if (range->UpperEndpoint == NULL) goto done;

	success = true;
	
done:
	if (!success) {
		delete range;
		return NULL;
	} else {
		tokIdx = idx;
		return range;
	}
}
MySizeConstraint* MyParser::ParseSizeConstraint(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MySizeConstraint* sc = new MySizeConstraint();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "SIZE")) {
		idx++;

		sc->Constraint = ParseConstraint(idx);
		if (sc->Constraint == NULL) goto done;

		success = true;
	}
	
done:
	if (!success) {
		delete sc;
		return NULL;
	} else {
		tokIdx = idx;
		return sc;
	}
}
MyPermittedAlphabet* MyParser::ParsePermittedAlphabet(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyPermittedAlphabet* pa = new MyPermittedAlphabet();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "FROM")) {
		idx++;

		pa->Constraint = ParseConstraint(idx);
		if (pa->Constraint == NULL) goto done;

		success = true;
	}
	
done:
	if (!success) {
		delete pa;
		return NULL;
	} else {
		tokIdx = idx;
		return pa;
	}
}
MyValueConstraint* MyParser::ParseValueConstraint(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyValueConstraint* vc = new MyValueConstraint();

	vc->Constraint = ParseConstraint(idx);
	if (vc->Constraint) { success = true; goto done; }

	vc->Empty = true;
	success = true;
	
done:
	if (!success) {
		delete vc;
		return NULL;
	} else {
		tokIdx = idx;
		return vc;
	}
}
MyPresenceConstraint* MyParser::ParsePresenceConstraint(int& tokIdx) {
	MyPresenceConstraint* pc = new MyPresenceConstraint();
	if (IsToken(tokIdx, TOKEN_RESERVED_WORD, "PRESENT") ||
		IsToken(tokIdx, TOKEN_RESERVED_WORD, "ABSENT") ||
		IsToken(tokIdx, TOKEN_RESERVED_WORD, "OPTIONAL")
	) {
		pc->Value.Set(m_Tokens.Get(tokIdx));
		tokIdx++;
	}
	return pc;
}
MyPatternConstraint* MyParser::ParsePatternConstraint(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyPatternConstraint* pc = new MyPatternConstraint();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "PATTERN")) {
		idx++;

		pc->Value = ParseValue(idx);
		if (pc->Value == NULL) goto done;

		success = true;
	}
	
done:
	if (!success) {
		delete pc;
		return NULL;
	} else {
		tokIdx = idx;
		return pc;
	}
}
MyComponentConstraint* MyParser::ParseComponentConstraint(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyComponentConstraint* cc = new MyComponentConstraint();

	cc->ValueConstraint = ParseValueConstraint(idx);
	if (cc->ValueConstraint == NULL) goto done;

	cc->PresenceConstraint = ParsePresenceConstraint(idx);
	if (cc->PresenceConstraint == NULL) goto done;

	success = true;
	
done:
	if (!success) {
		delete cc;
		return NULL;
	} else {
		tokIdx = idx;
		return cc;
	}
}
MyNamedConstraint* MyParser::ParseNamedConstraint(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyNamedConstraint* nc = new MyNamedConstraint();

	if (IsToken(idx, TOKEN_IDENTIFIER)) {
		nc->Identifier.Set(m_Tokens.Get(idx));
		idx++;

		nc->ComponentConstraint = ParseComponentConstraint(idx);
		if (nc->ComponentConstraint == NULL) goto done;

		success = true;
	}

done:
	if (!success) {
		delete nc;
		return NULL;
	} else {
		tokIdx = idx;
		return nc;
	}
}
MyTypeConstraints* MyParser::ParseTypeConstraints(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyTypeConstraints* tcs = new MyTypeConstraints();

	tcs->NamedConstraint = ParseNamedConstraint(idx);
	if (tcs->NamedConstraint == NULL) goto done;

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) {
		idx++;

		tcs->TypeConstraints = ParseTypeConstraints(idx);
		if (tcs->TypeConstraints == NULL) goto done;
	}
	success = true;

done:
	if (!success) {
		delete tcs;
		return NULL;
	} else {
		tokIdx = idx;
		return tcs;
	}
}
MyFullSpecification* MyParser::ParseFullSpecification(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyFullSpecification* fs = new MyFullSpecification();

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) {
		idx++;

		fs->TypeConstraints = ParseTypeConstraints(idx);
		if (fs->TypeConstraints == NULL) goto done;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

		success = true;
	}

done:
	if (!success) {
		delete fs;
		return NULL;
	} else {
		tokIdx = idx;
		return fs;
	}
}
MyPartialSpecification* MyParser::ParsePartialSpecification(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyPartialSpecification* ps = new MyPartialSpecification();

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) {
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_ELLIPSIS, "...")) goto done;
		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) goto done;

		ps->TypeConstraints = ParseTypeConstraints(idx);
		if (ps->TypeConstraints == NULL) goto done;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

		success = true;
	}

done:
	if (!success) {
		delete ps;
		return NULL;
	} else {
		tokIdx = idx;
		return ps;
	}
}
MyMultipleTypeConstraints* MyParser::ParseMultipleTypeConstraints(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyMultipleTypeConstraints* mtc = new MyMultipleTypeConstraints();

	mtc->FullSpecification = ParseFullSpecification(idx);
	if (mtc->FullSpecification) { success =true; goto done; }

	mtc->PartialSpecification = ParsePartialSpecification(idx);
	if (mtc->PartialSpecification) { success = true; goto done; }

done:
	if (!success) {
		delete mtc;
		return NULL;
	} else {
		tokIdx = idx;
		return mtc;
	}
}
MyInnerTypeConstraints* MyParser::ParseInnerTypeConstraints(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyInnerTypeConstraints* itc = new MyInnerTypeConstraints();

	if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "WITH")) goto done;

	if (IsToken(idx, TOKEN_RESERVED_WORD, "COMPONENT")) {
		idx++;

		itc->SingleTypeConstraint = ParseConstraint(idx);
		success = itc->SingleTypeConstraint != NULL;
	} else if (IsToken(idx, TOKEN_RESERVED_WORD, "COMPONENTS")) {
		idx++;

		itc->MultipleTypeConstraints = ParseMultipleTypeConstraints(idx);
		success = itc->MultipleTypeConstraints != NULL;
	}

done:
	if (!success) {
		delete itc;
		return NULL;
	} else {
		tokIdx = idx;
		return itc;
	}
}
MySubtypeElements* MyParser::ParseSubtypeElements(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MySubtypeElements* elems = new MySubtypeElements();

	elems->SingleValue = ParseValue(idx);
	if (elems->SingleValue) { success = true; goto done; }

	elems->ContainedSubtype = ParseContainedSubtype(idx);
	if (elems->ContainedSubtype) { success = true; goto done; }

	elems->ValueRange = ParseValueRange(idx);
	if (elems->ValueRange) { success = true; goto done; }

	elems->PermittedAlphabet = ParsePermittedAlphabet(idx);
	if (elems->PermittedAlphabet) { success = true; goto done; }

	elems->SizeConstraint = ParseSizeConstraint(idx);
	if (elems->SizeConstraint) { success = true; goto done; }

	elems->TypeConstraint = ParseType(idx);
	if (elems->TypeConstraint) { success = true; goto done; }

	elems->InnerTypeConstraints = ParseInnerTypeConstraints(idx);
	if (elems->InnerTypeConstraints) { success = true; goto done; }

	elems->PatternConstraint = ParsePatternConstraint(idx);
	if (elems->PatternConstraint) { success = true; goto done; }

done:
	if (!success) {
		delete elems;
		return NULL;
	} else {
		tokIdx = idx;
		return elems;
	}
}
MyUnions* MyParser::ParseUnions(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyUnions* us = new MyUnions();

	us->Intersections = ParseIntersections(idx);
	if (us->Intersections) { success = true; goto done; }

	us->UElems = ParseUnions(idx);
	if (us->UElems == NULL) goto done;

	us->UnionMark = ParseUnionMark(idx);
	if (us->UnionMark == NULL) goto done;

	us->Intersections = ParseIntersections(idx);
	if (us->Intersections == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete us;
		return NULL;
	} else {
		tokIdx = idx;
		return us;
	}
}
MyIntersections* MyParser::ParseIntersections(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyIntersections* inter = new MyIntersections();

	inter->IntersectionElements = ParseIntersectionElements(idx);
	if (inter->IntersectionElements) { success = true; goto done; }

	inter->IElems = ParseIntersections(idx);
	if (inter->IElems == NULL) goto done;

	inter->IntersectionMark = ParseIntersectionMark(idx);
	if (inter->IntersectionMark == NULL) goto done;

	inter->IntersectionElements = ParseIntersectionElements(idx);
	if (inter->IntersectionElements == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete inter;
		return NULL;
	} else {
		tokIdx = idx;
		return inter;
	}
}
MyIntersectionElements* MyParser::ParseIntersectionElements(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyIntersectionElements* interElems = new MyIntersectionElements();

	interElems->Elements = ParseElements(idx);
	if (interElems->Elements == NULL) goto done;

	interElems->Exclusions = ParseExclusions(idx);
	if (interElems->Exclusions) {
		interElems->Elems = interElems->Elements;
		interElems->Elements = NULL;
	}
	success = true;

done:
	if (!success) {
		delete interElems;
		return NULL;
	} else {
		tokIdx = idx;
		return interElems;
	}
}
MyConstraintSpec* MyParser::ParseConstraintSpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyConstraintSpec* spec = new MyConstraintSpec();

	spec->SubtypeConstraint = ParseElementSetSpecs(idx);
	if (spec->SubtypeConstraint) { success = true; goto done; }

	spec->GeneralConstraint = ParseGeneralConstraint(idx);
	if (spec->GeneralConstraint) { success = true; goto done; }

done:
	if (!success) {
		delete spec;
		return NULL;
	} else {
		tokIdx = idx;
		return spec;
	}
}

MyGeneralConstraint* MyParser::ParseGeneralConstraint(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyGeneralConstraint* c = new MyGeneralConstraint();

	c->UserDefinedConstraint = ParseUserDefinedConstraint(idx);
	if (c->UserDefinedConstraint) { success = true; goto done; }

	c->TableConstraint = ParseTableConstraint(idx);
	if (c->TableConstraint) { success = true; goto done; }

	c->ContentsConstraint = ParseContentsConstraint(idx);
	if (c->ContentsConstraint) { success = true; goto done; }

done:
	if (!success) {
		delete c;
		return NULL;
	} else {
		tokIdx = idx;
		return c;
	}
}
MyUserDefinedConstraint* MyParser::ParseUserDefinedConstraint(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyUserDefinedConstraintParameter* parameter = NULL;
	MyUserDefinedConstraint* c = new MyUserDefinedConstraint();

	if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "CONSTRAINED")) goto done;
	if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "BY")) goto done;
	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) goto done;

	parameter = ParseUserDefinedConstraintParameter(idx);
	if (parameter) {
		do {
			c->UserDefinedConstraintParameters.Add(parameter);

			if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) {
				idx++;
			}

			parameter = ParseUserDefinedConstraintParameter(idx);
		} while (true);
	}

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

	success = true;
	
done:
	if (!success) {
		delete c;
		return NULL;
	} else {
		tokIdx = idx;
		return c;
	}
}
MyUserDefinedConstraintParameter* MyParser::ParseUserDefinedConstraintParameter(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyUserDefinedConstraintParameter* parameter = new MyUserDefinedConstraintParameter();

	parameter->Governor = ParseGovernor(idx);
	if (parameter->Governor) {
		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ":")) goto done;

		parameter->Value = ParseValue(idx);
		if (parameter->Value) { success = true; goto done; }

		parameter->Object = ParseObject(idx);
		if (parameter->Object) { success = true; goto done; }

		// failed
		idx = tokIdx;
	}

	parameter->DefinedObjectSet = ParseDefinedObjectSet(idx);
	if (parameter->DefinedObjectSet) { success = true; goto done; }

	parameter->Type = ParseType(idx);
	if (parameter->Type) { success = true; goto done; }

	parameter->DefinedObjectClass = ParseDefinedObjectClass(idx);
	if (parameter->DefinedObjectClass) { success = true; goto done; }

done:
	if (!success) {
		delete parameter;
		return NULL;
	} else {
		tokIdx = idx;
		return parameter;
	}
}
MyTableConstraint* MyParser::ParseTableConstraint(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyTableConstraint* tc = new MyTableConstraint();

	tc->SimpleTableConstraint = ParseObjectSet(idx);
	if (tc->SimpleTableConstraint) { success = true; goto done; }

	tc->ComponentRelationConstraint = ParseComponentRelationConstraint(idx);
	if (tc->ComponentRelationConstraint) { success = true; goto done; }

done:
	if (!success) {
		delete tc;
		return NULL;
	} else {
		tokIdx = idx;
		return tc;
	}
}
MyComponentRelationConstraint* MyParser::ParseComponentRelationConstraint(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyAtNotation* atNotation = NULL;
	MyComponentRelationConstraint* c = new MyComponentRelationConstraint();

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) goto done;
	c->DefinedObjectSet = ParseDefinedObjectSet(idx);
	if (c->DefinedObjectSet == NULL) goto done;
	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) goto done;
	while (true) {
		atNotation = ParseAtNotation(idx);
		if (atNotation == NULL) break;
		c->AtNotations.Add(atNotation);

		if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) {
			idx++;
		} else {
			success = true;
			break;
		}
	}
	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

done:
	if (!success) {
		delete c;
		return NULL;
	} else {
		tokIdx = idx;
		return c;
	}
}
MyAtNotation* MyParser::ParseAtNotation(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyAtNotation* notation = new MyAtNotation();

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "@")) {
		idx++;

		if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ".")) {
			assert(false); // TODO:
		} else {
			notation->ComponentIdList = ParseComponentIdList(idx);
			if (notation->ComponentIdList) { success = true; goto done; }
		}
	}
	
done:
	if (!success) {
		delete notation;
		return NULL;
	} else {
		tokIdx = idx;
		return notation;
	}
}
MyComponentIdList* MyParser::ParseComponentIdList(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyComponentIdList* list = new MyComponentIdList();

	while (true) {
		if (IsToken(idx, TOKEN_IDENTIFIER)) {
			list->Identifiers.AddNew()->Set(m_Tokens.Get(idx));
			idx++;

			if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ".")) {
				idx++;
			} else {
				success = true;
				break;
			}
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
MyContentsConstraint* MyParser::ParseContentsConstraint(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyContentsConstraint* c = new MyContentsConstraint();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "CONTAINING")) {
		idx++;

		c->ContainingType = ParseType(idx);
		if (c->ContainingType == NULL) goto done;
	}

	if (IsToken(idx, TOKEN_RESERVED_WORD, "ENCODED")) {
		idx++;

		c->EncodedByValue = ParseValue(idx);
		if (c->EncodedByValue == NULL) goto done;
	}

	success = c->ContainingType != NULL || c->EncodedByValue != NULL;
	
done:
	if (!success) {
		delete c;
		return NULL;
	} else {
		tokIdx = idx;
		return c;
	}
}

MyElementSetSpecs* MyParser::ParseElementSetSpecs(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyElementSetSpecs* specs = new MyElementSetSpecs();

	specs->RootElementSetSpec = ParseElementSetSpec(idx);
	if (specs->RootElementSetSpec == NULL) goto done;

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) {
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_ELLIPSIS, "...")) goto done;
		specs->Ellipsis = true;

		if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) {
			idx++;

			specs->AdditionalElementSetSpec = ParseElementSetSpec(idx);
			if (specs->AdditionalElementSetSpec == NULL) goto done;
		}
	}
	success = true;
	
done:
	if (!success) {
		delete specs;
		return NULL;
	} else {
		tokIdx = idx;
		return specs;
	}

}
MyElementSetSpec* MyParser::ParseElementSetSpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyElementSetSpec* ess = new MyElementSetSpec();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "ALL")) {
		idx++;

		ess->AllExclusions = ParseExclusions(idx);
		if (ess->AllExclusions == NULL) goto done;

		success = true;
	} else {
		ess->Unions = ParseUnions(idx);
	}
	
done:
	if (!success) {
		delete ess;
		return NULL;
	} else {
		tokIdx = idx;
		return ess;
	}
}
MyExclusions* MyParser::ParseExclusions(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyExclusions* exs = new MyExclusions();

	if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "EXCEPT")) goto done;

	exs->Elements = ParseElements(idx);
	if (exs->Elements == NULL) goto done;

	success = true;
	
done:
	if (!success) {
		delete exs;
		return NULL;
	} else {
		tokIdx = idx;
		return exs;
	}
}
MyElements* MyParser::ParseElements(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyElements* elems = new MyElements();

	elems->SubtypeElements = ParseSubtypeElements(idx);
	if (elems->SubtypeElements) { success = true; goto done; }

	elems->ObjectSetElements = ParseObjectSetElements(idx);
	if (elems->ObjectSetElements) { success = true; goto done; }

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "(")) goto done;
	elems->ElementSetSpec = ParseElementSetSpec(idx);
	if (elems->ElementSetSpec) { success = true; goto done; }
	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ")")) goto done;

	success = true;

done:
	if (!success) {
		delete elems;
		return NULL;
	} else {
		tokIdx = idx;
		return elems;
	}
}
MyObjectSetElements* MyParser::ParseObjectSetElements(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjectSetElements* elems = new MyObjectSetElements();

	elems->Object = ParseObject(idx);
	if (elems->Object) { success = true; goto done; }

	elems->DefinedObjectSet = ParseDefinedObjectSet(idx);
	if (elems->DefinedObjectSet) { success = true; goto done; }

	elems->ObjectSetFromObjects = ParseObjectSetFromObjects(idx);
	if (elems->ObjectSetFromObjects) { success = true; goto done; }

	elems->ParameterizedObjectSet = ParseParameterizedObjectSet(idx);
	if (elems->ParameterizedObjectSet) { success = true; goto done; }

done:
	if (!success) {
		delete elems;
		return NULL;
	} else {
		tokIdx = idx;
		return elems;
	}
}

MyEnumerations* MyParser::ParseEnumerations(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyEnumerations* enums = new MyEnumerations();

	enums->RootEnumeration = ParseEnumeration(idx);
	if (enums->RootEnumeration == NULL) goto done;

	if (!IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) {
		success = true;
		goto done; // only RootEnumeration field
	}
	idx++;

	if (err = ExpectedTokenType(idx, TOKEN_ELLIPSIS, "...")) goto done;

	enums->ExceptionSpec = ParseExceptionSpec(idx);
	if (enums->ExceptionSpec == NULL) goto done;

	if (!IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) {
		success = true;
		goto done; // No AdditionalEnumeration
	}
	idx++;

	enums->AdditionalEnumeration = ParseEnumeration(idx);
	if (enums->AdditionalEnumeration == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete enums;
		return NULL;
	} else {
		tokIdx = idx;
		return enums;
	}
}
MyEnumeration* MyParser::ParseEnumeration(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyEnumeration* enu = new MyEnumeration();

	while (true) {
		MyEnumerationItem* item = ParseEnumerationItem(idx);
		if (item != NULL) {
			enu->EnumerationItems.Add(item);
			success = true;

			if (!IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) break;
			idx++;
		} else {
			break;
		}
	}

done:
	if (!success) {
		delete enu;
		return NULL;
	} else {
		tokIdx = idx;
		return enu;
	}
}
MyEnumerationItem* MyParser::ParseEnumerationItem(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyEnumerationItem* item = new MyEnumerationItem();

	
	item->NamedNumber = ParseNamedNumber(idx);
	if (item->NamedNumber != NULL) { success = true; goto done; }

	if (IsToken(idx, TOKEN_IDENTIFIER)) {
		item->Identifier.Set(m_Tokens.Get(idx));
		idx++;

		success = true;
	}
	
done:
	if (!success) {
		delete item;
		return NULL;
	} else {
		tokIdx = idx;
		return item;
	}
}
MyAlternativeTypeLists* MyParser::ParseAlternativeTypeLists(int& tokIdx) {
  int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyAlternativeTypeLists* list = new MyAlternativeTypeLists();

	list->RootAlternativeTypeList = ParseAlternativeTypeList(idx);
	if (list->RootAlternativeTypeList == NULL) goto done;

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) {
		idx++;

		list->ExtensionAndException = ParseExtensionAndException(idx);
		if (list->ExtensionAndException == NULL) goto done;

		list->ExtensionAdditionAlternatives = ParseExtensionAdditionAlternative(idx);
		if (list->ExtensionAdditionAlternatives == NULL) goto done;

		list->OptionalExtensionMarker = ParseOptionalExtensionMarker(idx);
		if (list->OptionalExtensionMarker == NULL) goto done;

		success = true;
	} else {
		success = true;
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
MyAlternativeTypeList* MyParser::ParseAlternativeTypeList(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyAlternativeTypeList* list = new MyAlternativeTypeList();

	while (true) {
		MyNamedType* typ = ParseNamedType(idx);
		if (typ != NULL) {
			list->NamedTypes.Add(typ);
			success = true;

			if (!IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) break;
			idx++;
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
MyExtensionAndException* MyParser::ParseExtensionAndException(int& tokIdx) {
  int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyExtensionAndException* extException = new MyExtensionAndException();

	if (IsToken(idx, TOKEN_ELLIPSIS, "...")) {
		idx++;

		extException->ExceptionSpec = ParseExceptionSpec(idx);
		//if (extException->ExceptionSpec == NULL) goto done;

		success = true;
	}

done:
	if (!success) {
		delete extException;
		return NULL;
	} else {
		tokIdx = idx;
		return extException;
	}
}
MyExtensionAdditionAlternative* MyParser::ParseExtensionAdditionAlternative(int& tokIdx) {
  int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyExtensionAdditionAlternative* extAddAlt = new MyExtensionAdditionAlternative();

	extAddAlt->ExtensionAdditionAlternativeGroup = ParseExtensionAdditionAlternativeGroup(idx);
	if (extAddAlt->ExtensionAdditionAlternativeGroup) {
		success = true;
		goto done;
	}

	extAddAlt->NamedType = ParseNamedType(idx);
	if (extAddAlt->NamedType) {
		success = true;
		goto done;
	}

done:
	if (!success) {
		delete extAddAlt;
		return NULL;
	} else {
		tokIdx = idx;
		return extAddAlt;
	}
}
MyExtensionAdditionGroup* MyParser::ParseExtensionAdditionGroup(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyExtensionAdditionGroup* group = new MyExtensionAdditionGroup();

	if (IsToken(idx, TOKEN_LEFT_VERSION_BRACKETS, "[[")) {
		idx++;

		group->VersionNumber = ParseVersionNumber(idx);
		if (group->VersionNumber == NULL) goto done;

		group->ComponentTypeList = ParseComponentTypeList(idx);
		if (group->ComponentTypeList == NULL) goto done;

		if (err = ExpectedTokenType(idx, TOKEN_RIGHT_VERSION_BRACKETS, "]]")) goto done;

		success = true;
	}

done:
	if (!success) {
		delete group;
		return NULL;
	} else {
		tokIdx = idx;
		return group;
	}
}
MyExtensionAdditionList* MyParser::ParseExtensionAdditionList(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyExtensionAdditionList* list = new MyExtensionAdditionList();

	while (true) {
		MyExtensionAddition* add = ParseExtensionAddition(idx);
		if (add != NULL) {
			list->ExtensionAdditions.Add(add);
			success = true;

			if (!IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) break;
			idx++;
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
MyExtensionAddition* MyParser::ParseExtensionAddition(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyExtensionAddition* add = new MyExtensionAddition();

	add->ComponentType = ParseComponentType(idx);
	if (add->ComponentType) { success = true; goto done; }

	add->ExtensionAdditionGroup = ParseExtensionAdditionGroup(idx);
	if (add->ExtensionAdditionGroup) { success = true; goto done; }

done:
	if (!success) {
		delete add;
		return NULL;
	} else {
		tokIdx = idx;
		return add;
	}
}
MyExtensionAdditions* MyParser::ParseExtensionAdditions(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyExtensionAdditions* adds = new MyExtensionAdditions();

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) {
		idx++;

		adds->ExtensionAdditionList = ParseExtensionAdditionList(idx);
		if (adds->ExtensionAdditionList == NULL) goto done;

		success = true;
	} else {
		adds->Empty = true;
		success = true;
	}

done:
	if (!success) {
		delete adds;
		return NULL;
	} else {
		tokIdx = idx;
		return adds;
	}
}
MyOptionalExtensionMarker* MyParser::ParseOptionalExtensionMarker(int& tokIdx) {
  int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyOptionalExtensionMarker* marker = new MyOptionalExtensionMarker();

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) {
		idx++;
		marker->Empty = false;

		if (err = ExpectedTokenType(idx, TOKEN_ELLIPSIS, "...")) goto done;

		success = true;

	} else {
		marker->Empty = true;
		success = true;
	}

done:
	if (!success) {
		delete marker;
		return NULL;
	} else {
		tokIdx = idx;
		return marker;
	}
}
MyExtensionEndMarker* MyParser::ParseExtensionEndMarker(int& tokIdx) {
  int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyExtensionEndMarker* marker = new MyExtensionEndMarker();

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) {
		idx++;
		if (err = ExpectedTokenType(idx, TOKEN_ELLIPSIS, "...")) goto done;
		success = true;
	}

done:
	if (!success) {
		delete marker;
		return NULL;
	} else {
		tokIdx = idx;
		return marker;
	}
}

MyNamedType* MyParser::ParseNamedType(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyNamedType* typ = new MyNamedType();

	if (IsToken(idx, TOKEN_IDENTIFIER)) {
		typ->Identifier.Set(m_Tokens.Get(idx));
		idx++;

		typ->Type = ParseType(idx);
		if (typ->Type == NULL) goto done;

		success = true;
	}

done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}
MyExceptionSpec* MyParser::ParseExceptionSpec(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyExceptionSpec* spec = new MyExceptionSpec();

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "!")) {
		idx++;

		spec->ExceptionIdentification = ParseExceptionIdentification(idx);
		if (spec->ExceptionIdentification == NULL) goto done;

		success = true;
	} else {
		spec->Empty = true;
		success = true;
	}

done:
	if (!success) {
		delete spec;
		return NULL;
	} else {
		tokIdx = idx;
		return spec;
	}
}
MyExceptionIdentification* MyParser::ParseExceptionIdentification(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyExceptionIdentification* id = new MyExceptionIdentification();

	id->SignedNumber = ParseSignedNumber(idx);
	if (id->SignedNumber) { success = true; goto done; }

	id->DefinedValue = ParseDefinedValue(idx);
	if (id->DefinedValue) { success = true; goto done; }

	id->Type = ParseType(idx);
	if (id->Type) {

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ":")) goto done;

		id->Value = ParseValue(idx);
		if (id->Value) {
			success = true;
			goto done;
		}
	}

done:
	if (!success) {
		delete id;
		return NULL;
	} else {
		tokIdx = idx;
		return id;
	}
}
MyExtensionAdditionAlternativeGroup* MyParser::ParseExtensionAdditionAlternativeGroup(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyExtensionAdditionAlternativeGroup* group = new MyExtensionAdditionAlternativeGroup();

	if (IsToken(idx, TOKEN_LEFT_VERSION_BRACKETS, "[[")) {
		idx++;

		group->VersionNumber = ParseVersionNumber(idx);
		if (group->VersionNumber == NULL) goto done;

		group->AlternativeTypeList = ParseAlternativeTypeList(idx);
		if (group->AlternativeTypeList == NULL) goto done;

		if (!IsToken(idx, TOKEN_RIGHT_VERSION_BRACKETS, "]]")) goto done;
		idx++;

		success = true;
	}

done:
	if (!success) {
		delete group;
		return NULL;
	} else {
		tokIdx = idx;
		return group;
	}
}
MyVersionNumber* MyParser::ParseVersionNumber(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyVersionNumber* versionNumber = new MyVersionNumber();

	if (IsToken(idx, TOKEN_NUMBER)) {
		versionNumber->Number.Set(m_Tokens.Get(idx));
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ":")) goto done;

		success = true;
	} else {
		success = true; // Empty
	}

done:
	if (!success) {
		delete versionNumber;
		return NULL;
	} else {
		tokIdx = idx;
		return versionNumber;
	}
}

MySignedNumber* MyParser::ParseSignedNumber(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MySignedNumber* num = new MySignedNumber();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "-")) {
		num->Negative = true;
		idx++;
	}

	if (IsToken(idx, TOKEN_NUMBER)) {
		num->Number.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
	}

done:
	if (!success) {
		delete num;
		return NULL;
	} else {
		tokIdx = idx;
		return num;
	}
}
MyDefinedValue* MyParser::ParseDefinedValue(int& tokIdx) {
	return NULL;
}

MyValue* MyParser::ParseValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyValue* val = new MyValue();

	val->BuiltinValue = ParseBuiltinValue(idx);
	if (val->BuiltinValue) { success = true; goto done; }

	val->ReferencedValue = ParseReferencedValue(idx);
	if (val->ReferencedValue) { success = true; goto done; }

	val->ObjectClassFieldValue = ParseObjectClassFieldValue(idx);
	if (val->ObjectClassFieldValue) { success = true; goto done; }
	
done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MyBuiltinValue* MyParser::ParseBuiltinValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyBuiltinValue* val = new MyBuiltinValue();

	val->BitStringValue = ParseBitStringValue(idx);
	if (val->BitStringValue) { success = true; goto done; }

	val->BooleanValue = ParseBooleanValue(idx);
	if (val->BooleanValue) { success = true; goto done; }

	val->ChoiceValue = ParseChoiceValue(idx);
	if (val->ChoiceValue) { success = true; goto done; }

	val->EnumeratedValue = ParseEnumeratedValue(idx);
	if (val->EnumeratedValue) { success = true; goto done; }

	// TODO:
	//val->ExternalValue = ParseExternalValue(idx);
	//if (val->ExternalValue) { success = true; goto done; }

	val->IntegerValue = ParseIntegerValue(idx);
	if (val->IntegerValue) { success = true; goto done; }

	val->NullValue = ParseNullValue(idx);
	if (val->NullValue) { success = true; goto done; }

	val->ObjectIDValue = ParseObjectIDValue(idx);
	if (val->ObjectIDValue) { success = true; goto done; }

	val->OctetStringValue = ParseOctetStringValue(idx);
	if (val->OctetStringValue) { success = true; goto done; }

	val->RealValue = ParseRealValue(idx);
	if (val->RealValue) { success = true; goto done; }

	val->SequenceValue = ParseSequenceValue(idx);
	if (val->SequenceValue) { success = true; goto done; }

	val->SequenceOfValue = ParseSequenceOfValue(idx);
	if (val->SequenceOfValue) { success = true; goto done; }

	val->SetValue = ParseSetValue(idx);
	if (val->SetValue) { success = true; goto done; }

	val->SetOfValue = ParseSetOfValue(idx);
	if (val->SetOfValue) { success = true; goto done; }

	val->TaggedValue = ParseTaggedValue(idx);
	if (val->TaggedValue) { success = true; goto done; }

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MyBitStringValue* MyParser::ParseBitStringValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyBitStringValue* val = new MyBitStringValue();

	if (IsToken(idx, TOKEN_BSTRING)) {
		val->BString.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
		goto done;
	}

	if (IsToken(idx, TOKEN_HEXSTRING)) {
		val->HString.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
		goto done;
	}

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) {
		idx++;

		if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) {
			idx++;
			success = true;
			goto done;
		}

		val->IdentifierList = ParseIdentifierList(idx);
		if (val->IdentifierList == NULL) goto done;

		if (err =ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

		success = true;
		goto done;
	}

	if (IsToken(idx, TOKEN_RESERVED_WORD, "CONTAINING")) {
		idx++;

		val->ContainingValue = ParseValue(idx);
		if (val->ContainingValue == NULL) goto done;

		success = true;
		goto done;
	}
	
done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MyBooleanValue* MyParser::ParseBooleanValue(int& tokIdx) {
	if (IsToken(tokIdx, TOKEN_RESERVED_WORD, "TRUE") || IsToken(tokIdx, TOKEN_RESERVED_WORD, "FALSE")) {
		MyBooleanValue* val = new MyBooleanValue();
		val->Value = m_Tokens.Get(tokIdx)->Equals("TRUE");
		tokIdx++;
		return val;
	}
	return NULL;
}
MyChoiceValue* MyParser::ParseChoiceValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyChoiceValue* val = new MyChoiceValue();

	if (IsToken(idx, TOKEN_IDENTIFIER)) {
		val->Identifier.Set(m_Tokens.Get(idx));
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ":")) goto done;

		val->Value = ParseValue(idx);
		if (val->Value == NULL) goto done;

		success = true;
	}
	
done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MyEnumeratedValue* MyParser::ParseEnumeratedValue(int& tokIdx) {
	if (IsToken(tokIdx, TOKEN_IDENTIFIER)) {
		MyEnumeratedValue* val= new MyEnumeratedValue();
		val->Identifier.Set(m_Tokens.Get(tokIdx));
		tokIdx++;
		return val;
	}
	return NULL;
}
MyExternalValue* MyParser::ParseExternalValue(int& tokIdx) {
	return ParseSequenceValue(tokIdx);
}
MyIntegerValue* MyParser::ParseIntegerValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyIntegerValue* val = new MyIntegerValue();

	val->SignedNumber = ParseSignedNumber(idx);
	if (val->SignedNumber) { success = true; goto done; }

	if (IsToken(idx, TOKEN_IDENTIFIER)) {
		val->Identifier.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
		goto done;
	}

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MyNullValue* MyParser::ParseNullValue(int& tokIdx) {
	if (IsToken(tokIdx, TOKEN_RESERVED_WORD, "NULL")) {
		tokIdx++;
		return new MyNullValue();
	}
	return NULL;
}
MyOctetStringValue* MyParser::ParseOctetStringValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyOctetStringValue* val = new MyOctetStringValue();

	if (IsToken(idx, TOKEN_BSTRING)) {
		val->BString.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
		goto done;
	}

	if (IsToken(idx, TOKEN_HEXSTRING)) {
		val->HString.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
		goto done;
	}

	if (IsToken(idx, TOKEN_RESERVED_WORD, "CONTAINING")) {
		idx++;

		val->ContainingValue = ParseValue(idx);
		if (val->ContainingValue == NULL) goto done;

		success = true;
		goto done;
	}

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MyRealValue* MyParser::ParseRealValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyRealValue* val = new MyRealValue();

	val->NumericValue = ParseNumericRealValue(idx);
	if (val->NumericValue) { success = true; goto done; }

	val->SpecialValue = ParseSpecialRealValue(idx);
	if (val->SpecialValue) { success = true; goto done; }

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MySequenceValue* MyParser::ParseSequenceValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MySequenceValue* val = new MySequenceValue();

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) {
		idx++;

		if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) {
			idx++;
			success = true;
			goto done;
		}

		val->ComponentValueList = ParseComponentValueList(idx);
		if (val->ComponentValueList == NULL) goto done;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

		success = true;
	}

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MySequenceOfValue* MyParser::ParseSequenceOfValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MySequenceOfValue* val = new MySequenceOfValue();

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) {
		idx++;

		if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) {
			idx++;
			success = true;
			goto done;
		}

		val->ValueList = ParseValueList(idx);
		if (val->ValueList != NULL) {
			// nothing
		} else {
			val->NamedValueList = ParseNamedValueList(idx);
			if (val->NamedValueList == NULL) goto done;
		}

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

		success = true;
	}

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MySetValue* MyParser::ParseSetValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MySetValue* val = new MySetValue();

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) {
		idx++;

		if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) {
			idx++;
			success = true;
			goto done;
		}

		val->ComponentValueList = ParseComponentValueList(idx);
		if (val->ComponentValueList == NULL) goto done;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

		success = true;
	}

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MySetOfValue* MyParser::ParseSetOfValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MySetOfValue* val = new MySetOfValue();

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) {
		idx++;

		if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) {
			idx++;
			success = true;
			goto done;
		}

		val->ValueList = ParseValueList(idx);
		if (val->ValueList != NULL) {
			// nothing
		} else {
			val->NamedValueList = ParseNamedValueList(idx);
			if (val->NamedValueList == NULL) goto done;
		}

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

		success = true;
	}

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MyTaggedValue* MyParser::ParseTaggedValue(int& tokIdx) {
	return ParseValue(tokIdx);
}
MyValueSet* MyParser::ParseValueSet(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyValueSet* val = new MyValueSet();

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) {
		idx++;

		val->ElementSetSpecs = ParseElementSetSpecs(idx);
		if (val->ElementSetSpecs == NULL) goto done;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

		success = true;
	}

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MyObject* MyParser::ParseObject(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObject* obj = new MyObject();

	obj->DefinedObject = ParseDefinedObject(idx);
	if (obj->DefinedObject) { success = true; goto done; }

	obj->ObjectDefn = ParseObjectDefn(idx);
	if (obj->ObjectDefn) { success = true; goto done; }
	
	obj->ObjectFromObject = ParseObjectFromObject(idx);
	if (obj->ObjectFromObject) { success = true; goto done; }

	obj->ParameterizedObject = ParseParameterizedObject(idx);
	if (obj->ParameterizedObject) { success = true; goto done; }

done:
	if (!success) {
		delete obj;
		return NULL;
	} else {
		tokIdx = idx;
		return obj;
	}
}
MyObjectDefn* MyParser::ParseObjectDefn(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjectDefn* obj = new MyObjectDefn();

	obj->DefaultSyntax = ParseDefaultSyntax(idx);
	if (obj->DefaultSyntax) { success = true; goto done; }
	
	obj->DefinedSyntax = ParseDefinedSyntax(idx);
	if (obj->DefinedSyntax) { success = true; goto done; }

done:
	if (!success) {
		delete obj;
		return NULL;
	} else {
		tokIdx = idx;
		return obj;
	}
}
MyObjectSet* MyParser::ParseObjectSet(int& tokIdx) {
	assert(false);
	return 0;
}
MyObjectClass* MyParser::ParseObjectClass(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjectClass* oc = new MyObjectClass();

	oc->DefinedObjectClass = ParseDefinedObjectClass(idx);
	if (oc->DefinedObjectClass) { success = true; goto done; }

	oc->ObjectClassDefn = ParseObjectClassDefn(idx);
	if (oc->ObjectClassDefn) { success = true; goto done; }

	oc->ParameterizedObjectClass = ParseParameterizedObjectClass(idx);
	if (oc->ParameterizedObjectClass) { success = true; goto done; }

done:
	if (!success) {
		delete oc;
		return NULL;
	} else {
		tokIdx = idx;
		return oc;
	}
}
MyObjectClassDefn* MyParser::ParseObjectClassDefn(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyFieldSpec* fs = NULL;
	MyObjectClassDefn* ocd = new MyObjectClassDefn();

	if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "CLASS")) goto done;
	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) goto done;

	while (true) {
		fs = ParseFieldSpec(idx);
		if (fs != NULL) {
			ocd->FieldSpecs.Add(fs);

			if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) {
				idx++;
			} else {
				break;
			}
		} else {
			goto done;
		}
	}

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

	ocd->WithSyntaxSpec = ParseWithSyntaxSpec(idx); // optional

	success = true;

done:
	if (!success) {
		delete ocd;
		return NULL;
	} else {
		tokIdx = idx;
		return ocd;
	}

}
MyNamedNumberList* MyParser::ParseNamedNumberList(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyNamedNumberList* list = new MyNamedNumberList();

	while (true) {
		MyNamedNumber* num = ParseNamedNumber(idx);
		if (num != NULL) {
			list->NamedNumbers.Add(num);
			success = true;

			if (!IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) break;
			idx++;
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
MyNamedNumber* MyParser::ParseNamedNumber(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyNamedNumber* namedNumber = new MyNamedNumber();

	if (IsToken(idx, TOKEN_IDENTIFIER)) {
		namedNumber->Identifier.Set(m_Tokens.Get(idx));
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "(")) goto done;

		namedNumber->SignedNumber = ParseSignedNumber(idx);
		if (namedNumber->SignedNumber == NULL) {
			namedNumber->DefinedValue = ParseDefinedValue(idx);
			if (namedNumber->DefinedValue == NULL) goto done;
		}

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "(")) goto done;

		success = true;
	}
	
done:
	if (!success) {
		delete namedNumber;
		return NULL;
	} else {
		tokIdx = idx;
		return namedNumber;
	}
}
MyIdentifierList* MyParser::ParseIdentifierList(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyIdentifierList* list = new MyIdentifierList();

	while (true) {
		if (IsToken(idx, TOKEN_IDENTIFIER)) {
			list->Identifiers.AddNew()->Set(m_Tokens.Get(idx));
			idx++;
			success = true;

			if (!IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) break;
			idx++;
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
MyNumericRealValue* MyParser::ParseNumericRealValue(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyNumericRealValue* val = new MyNumericRealValue();

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "-")) {
		val->Negative = true;
		idx++;
	}

	if (IsToken(idx, TOKEN_REAL_NUMBER)) {
		val->RealNumber.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
		goto done;
	}

	val->SequenceValue = ParseSequenceValue(idx);
	if (val->SequenceValue) { success = true; }

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MySpecialRealValue* MyParser::ParseSpecialRealValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MySpecialRealValue* val = new MySpecialRealValue();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "PLUS")) {
		idx++;
		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "-")) goto done;
		if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "INFINITY")) goto done;
		val->Value.Set("PLUS-INFINITY");
		success = true;
	} else if (IsToken(idx, TOKEN_RESERVED_WORD, "MINUS")) {
		idx++;
		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "-")) goto done;
		if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "INFINITY")) goto done;
		val->Value.Set("MINUS-INFINITY");
		success = true;
	}

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MyNamedValue* MyParser::ParseNamedValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyNamedValue* val = new MyNamedValue();

	if (IsToken(idx, TOKEN_IDENTIFIER)) {
		val->Identifier.Set(m_Tokens.Get(idx));
		idx++;

		val->Value = ParseValue(idx);
		if (val->Value == NULL) goto done;

		success = true;
	}

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MyComponentValueList* MyParser::ParseComponentValueList(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyComponentValueList* list = new MyComponentValueList();

	while (true) {
		MyNamedValue* namedValue = ParseNamedValue(idx);
		if (namedValue != NULL) {
			list->NamedValues.Add(namedValue);
			success = true;

			if (!IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) break;
			idx++;
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
MyValueList* MyParser::ParseValueList(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyValueList* list = new MyValueList();

	while (true) {
		MyValue* val = ParseValue(idx);
		if (val != NULL) {
			list->Values.Add(val);
			success = true;

			if (!IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) break;
			idx++;
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
MyNamedValueList* MyParser::ParseNamedValueList(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyNamedValueList* list = new MyNamedValueList();

	while (true) {
		MyNamedValue* val = ParseNamedValue(idx);
		if (val != NULL) {
			list->NamedValues.Add(val);
			success = true;

			if (!IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) break;
			idx++;
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
MyReferencedValue* MyParser::ParseReferencedValue(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyReferencedValue* val = new MyReferencedValue();

	val->DefinedValue = ParseDefinedValue(idx);
	if (val->DefinedValue) { success = true; goto done; }

	val->ValueFromObject = ParseValueFromObject(idx);
	if (val->ValueFromObject) { success = true; goto done; }

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MyValueFromObject* MyParser::ParseValueFromObject(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyValueFromObject* obj = new MyValueFromObject();

	obj->ReferencedObjects = ParseReferencedObjects(idx);
	if (obj->ReferencedObjects == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ".")) goto done;

	obj->FieldName = ParseFieldName(idx);
	if (obj->FieldName == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete obj;
		return NULL;
	} else {
		tokIdx = idx;
		return obj;
	}
}
MyObjectClassFieldValue* MyParser::ParseObjectClassFieldValue(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjectClassFieldValue* val = new MyObjectClassFieldValue();

	val->OpenTypeFieldVal = ParseOpenTypeFieldVal(idx);
	if (val->OpenTypeFieldVal) { success = true; goto done; }

	val->FixedTypeFieldVal = ParseFixedTypeFieldVal(idx);
	if (val->FixedTypeFieldVal) { success = true; goto done; }

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MyOpenTypeFieldVal* MyParser::ParseOpenTypeFieldVal(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyOpenTypeFieldVal* val = new MyOpenTypeFieldVal();

	val->Type = ParseType(idx);
	if (val->Type == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ":")) goto done;

	val->Value = ParseValue(idx);
	if (val->Value == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MyFixedTypeFieldVal* MyParser::ParseFixedTypeFieldVal(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyFixedTypeFieldVal* val = new MyFixedTypeFieldVal();

	val->BuiltinValue = ParseBuiltinValue(idx);
	if (val->BuiltinValue) { success = true; goto done; }

	val->ReferencedValue = ParseReferencedValue(idx);
	if (val->ReferencedValue) { success = true; goto done; }

done:
	if (!success) {
		delete val;
		return NULL;
	} else {
		tokIdx = idx;
		return val;
	}
}
MyObjectFromObject* MyParser::ParseObjectFromObject(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjectFromObject* ofo = new MyObjectFromObject();

	ofo->ReferencedObjects = ParseReferencedObjects(idx);
	if (ofo->ReferencedObjects == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ".")) goto done;

	ofo->FieldName = ParseFieldName(idx);
	if (ofo->FieldName == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete ofo;
		return NULL;
	} else {
		tokIdx = idx;
		return ofo;
	}
}
MyObjectSetFromObjects* MyParser::ParseObjectSetFromObjects(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjectSetFromObjects* objs = new MyObjectSetFromObjects();

	objs->ReferencedObjects = ParseReferencedObjects(idx);
	if (objs->ReferencedObjects == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ".")) goto done;

	objs->FieldName = ParseFieldName(idx);
	if (objs->FieldName == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete objs;
		return NULL;
	} else {
		tokIdx = idx;
		return objs;
	}
}
MyLiteral* MyParser::ParseLiteral(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyLiteral* literal = new MyLiteral();

	if (IsToken(idx, TOKEN_TYPE_REF)) {
		literal->Value.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
		goto done;
	}

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) {
		idx++;
		literal->Value.Set(",");
		success = true;
		goto done;
	}

done:
	if (!success) {
		delete literal;
		return NULL;
	} else {
		tokIdx = idx;
		return literal;
	}
}
MyRequiredToken* MyParser::ParseRequiredToken(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyRequiredToken* rt = new MyRequiredToken();

	rt->Literal = ParseLiteral(idx);
	if (rt->Literal) { success = true; goto done; }
	
	rt->PrimitiveFieldName = ParsePrimitiveFieldName(idx);
	if (rt->PrimitiveFieldName) { success = true; goto done; }

done:
	if (!success) {
		delete rt;
		return NULL;
	} else {
		tokIdx = idx;
		return rt;
	}
}
MyOptionalGroup* MyParser::ParseOptionalGroup(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyTokenOrGroupSpec* tgs = NULL;
	MyOptionalGroup* og = new MyOptionalGroup();

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "[")) goto done;

	while (true) {
		tgs = ParseTokenOrGroupSpec(idx);
		if (tgs) {
			og->TokenOrGroupSpecs.Add(tgs);
		} else {
			break;
		}
	}

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "]")) goto done;

	success = true;

done:
	if (!success) {
		delete og;
		return NULL;
	} else {
		tokIdx = idx;
		return og;
	}
}
MyTokenOrGroupSpec* MyParser::ParseTokenOrGroupSpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyTokenOrGroupSpec* tgs = new MyTokenOrGroupSpec();

	tgs->RequiredToken = ParseRequiredToken(idx);
	if (tgs->RequiredToken) { success = true; goto done; }
	
	tgs->OptionalGroup = ParseOptionalGroup(idx);
	if (tgs->OptionalGroup) { success = true; goto done; }

done:
	if (!success) {
		delete tgs;
		return NULL;
	} else {
		tokIdx = idx;
		return tgs;
	}
}
MySyntaxList* MyParser::ParseSyntaxList(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyTokenOrGroupSpec* tgs = NULL;
	MySyntaxList* list = new MySyntaxList();

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) goto done;

	while (true) {
		tgs = ParseTokenOrGroupSpec(idx);
		if (tgs) {
			list->TokenOrGroupSpecs.Add(tgs);
		} else {
			break;
		}
	}

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

	success = true;

done:
	if (!success) {
		delete list;
		return NULL;
	} else {
		tokIdx = idx;
		return list;
	}
}
MyWithSyntaxSpec* MyParser::ParseWithSyntaxSpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyWithSyntaxSpec* spec = new MyWithSyntaxSpec();

	if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "WITH")) goto done;
	if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "SYNTAX")) goto done;

	spec->SyntaxList = ParseSyntaxList(idx);
	if (spec->SyntaxList == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete spec;
		return NULL;
	} else {
		tokIdx = idx;
		return spec;
	}
}

MyDefinedObject* MyParser::ParseDefinedObject(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyDefinedObject* obj = new MyDefinedObject();

	obj->ExternalObjectReference = ParseExternalObjectReference(idx);
	if (obj->ExternalObjectReference) { success = true; goto done; }

	if (IsToken(idx, TOKEN_IDENTIFIER)) {
		obj->ObjectReference.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
	}

done:
	if (!success) {
		delete obj;
		return NULL;
	} else {
		tokIdx = idx;
		return obj;
	}
}
MyDefinedObjectSet* MyParser::ParseDefinedObjectSet(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyDefinedObjectSet* objSet = new MyDefinedObjectSet();

	objSet->ExternalObjectSetReference = ParseExternalObjectSetReference(idx);
	if (objSet->ExternalObjectSetReference) { success = true; goto done; }

	if (IsToken(idx, TOKEN_TYPE_REF)) {
		objSet->ObjectSetReference.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
	}

done:
	if (!success) {
		delete objSet;
		return NULL;
	} else {
		tokIdx = idx;
		return objSet;
	}
}
MyDefinedObjectClass* MyParser::ParseDefinedObjectClass(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyDefinedObjectClass* objClass = new MyDefinedObjectClass();

	objClass->ExternalObjectClassReference = ParseExternalObjectClassReference(idx);
	if (objClass->ExternalObjectClassReference) { success = true; goto done; }

	objClass->UsefulObjectClassReference = ParseUsefulObjectClassReference(idx);
	if (objClass->UsefulObjectClassReference) { success = true; goto done; }

	if (IsToken(idx, TOKEN_TYPE_REF)) {
		objClass->ObjectClassReference.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
	}

done:
	if (!success) {
		delete objClass;
		return NULL;
	} else {
		tokIdx = idx;
		return objClass;
	}
}
MyExternalObjectClassReference* MyParser::ParseExternalObjectClassReference(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyExternalObjectClassReference* extObjClassRef = new MyExternalObjectClassReference();

	if (IsToken(idx, TOKEN_MODULE_REF)) {
		extObjClassRef->ModuleReference.Set(m_Tokens.Get(idx));
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ".")) goto done;

		if (IsToken(idx, TOKEN_TYPE_REF)) {
			extObjClassRef->ObjectClassReference.Set(m_Tokens.Get(idx));
			idx++;

			success = true;
		} else {
			goto done; // failed
		}
	}

done:
	if (!success) {
		delete extObjClassRef;
		return NULL;
	} else {
		tokIdx = idx;
		return extObjClassRef;
	}
}
MyUsefulObjectClassReference* MyParser::ParseUsefulObjectClassReference(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyUsefulObjectClassReference* usefulObjClassRef = new MyUsefulObjectClassReference();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "TYPE-IDENTIFIER")) {
		usefulObjClassRef->Value.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
	} else if (IsToken(idx, TOKEN_RESERVED_WORD, "ABSTRACT-SYNTAX")) {
		usefulObjClassRef->Value.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
	}

done:
	if (!success) {
		delete usefulObjClassRef;
		return NULL;
	} else {
		tokIdx = idx;
		return usefulObjClassRef;
	}
}
MyFieldName* MyParser::ParseFieldName(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyFieldName* name = new MyFieldName();

	name->PrimitiveFieldName = ParsePrimitiveFieldName(idx);
	if (name->PrimitiveFieldName == NULL) goto done;

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ".")) goto done;

	success = true;
	
done:
	if (!success) {
		delete name;
		return NULL;
	} else {
		tokIdx = idx;
		return name;
	}
}
MyPrimitiveFieldName* MyParser::ParsePrimitiveFieldName(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyPrimitiveFieldName* name = new MyPrimitiveFieldName();

	assert(false); // TODO:
	
done:
	if (!success) {
		delete name;
		return NULL;
	} else {
		tokIdx = idx;
		return name;
	}
}
MyExternalObjectReference* MyParser::ParseExternalObjectReference(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyExternalObjectReference* ref = new MyExternalObjectReference();

	if (IsToken(idx, TOKEN_MODULE_REF)) {
		ref->ModuleReference.Set(m_Tokens.Get(idx));
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ".")) goto done;

		if (IsToken(idx, TOKEN_IDENTIFIER)) {
			ref->ObjectReference.Set(m_Tokens.Get(idx));
			idx++;
			success = true;
		}
	}
	
done:
	if (!success) {
		delete ref;
		return NULL;
	} else {
		tokIdx = idx;
		return ref;
	}
}
MyExternalObjectSetReference* MyParser::ParseExternalObjectSetReference(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyExternalObjectSetReference* ref = new MyExternalObjectSetReference();

	if (IsToken(idx, TOKEN_MODULE_REF)) {
		ref->ModuleReference.Set(m_Tokens.Get(idx));
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ".")) goto done;

		if (IsToken(idx, TOKEN_TYPE_REF)) {
			ref->ObjectSetReference.Set(m_Tokens.Get(idx));
			idx++;
			success = true;
		}
	}
	
done:
	if (!success) {
		delete ref;
		return NULL;
	} else {
		tokIdx = idx;
		return ref;
	}
}
MyParameterizedObject* MyParser::ParseParameterizedObject(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyParameterizedObject* obj = new MyParameterizedObject();

	obj->DefinedObject = ParseDefinedObject(idx);
	if (obj->DefinedObject == NULL) goto done;

	obj->ActualParameterList = ParseActualParameterList(idx);
	if (obj->ActualParameterList == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete obj;
		return NULL;
	} else {
		tokIdx = idx;
		return obj;
	}
}
MyParameterizedObjectSet* MyParser::ParseParameterizedObjectSet(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyParameterizedObjectSet* objSet = new MyParameterizedObjectSet();

	objSet->DefinedObjectSet = ParseDefinedObjectSet(idx);
	if (objSet->DefinedObjectSet == NULL) goto done;

	objSet->ActualParameterList = ParseActualParameterList(idx);
	if (objSet->ActualParameterList == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete objSet;
		return NULL;
	} else {
		tokIdx = idx;
		return objSet;
	}
}
MyParameterizedObjectClass* MyParser::ParseParameterizedObjectClass(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyParameterizedObjectClass* poc = new MyParameterizedObjectClass();

	poc->DefinedObjectClass = ParseDefinedObjectClass(idx);
	if (poc->DefinedObjectClass == NULL) goto done;

	poc->ActualParameterList = ParseActualParameterList(idx);
	if (poc->ActualParameterList == NULL) goto done;

	success = true;

done:
	if (!success) {
		delete poc;
		return NULL;
	} else {
		tokIdx = idx;
		return poc;
	}
}

MyFieldSpec* MyParser::ParseFieldSpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyFieldSpec* spec = new MyFieldSpec();

	spec->TypeFieldSpec = ParseTypeFieldSpec(idx);
	if (spec->TypeFieldSpec) { success = true; goto done; }

	spec->FixedTypeValueFieldSpec = ParseFixedTypeValueFieldSpec(idx);
	if (spec->FixedTypeValueFieldSpec) { success = true; goto done; }

	spec->VariableTypeValueFieldSpec = ParseVariableTypeValueFieldSpec(idx);
	if (spec->VariableTypeValueFieldSpec) { success = true; goto done; }

	spec->FixedTypeValueSetFieldSpec = ParseFixedTypeValueSetFieldSpec(idx);
	if (spec->FixedTypeValueSetFieldSpec) { success = true; goto done; }

	spec->VariableTypeValueSetFieldSpec = ParseVariableTypeValueSetFieldSpec(idx);
	if (spec->VariableTypeValueSetFieldSpec) { success = true; goto done; }

	spec->ObjectFieldSpec = ParseObjectFieldSpec(idx);
	if (spec->ObjectFieldSpec) { success = true; goto done; }

	spec->ObjectSetFieldSpec = ParseObjectSetFieldSpec(idx);
	if (spec->ObjectSetFieldSpec) { success = true; goto done; }

done:
	if (!success) {
		delete spec;
		return NULL;
	} else {
		tokIdx = idx;
		return spec;
	}
}
MyTypeFieldSpec* MyParser::ParseTypeFieldSpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyTypeFieldSpec* spec = new MyTypeFieldSpec();

	if (!ParseTypeFieldReference(idx, &spec->TypeFieldReference)) goto done;
	spec->TypeOptionalitySpec = ParseTypeOptionalitySpec(idx); // Optional
	success = true;

done:
	if (!success) {
		delete spec;
		return NULL;
	} else {
		tokIdx = idx;
		return spec;
	}
}
MyTypeOptionalitySpec* MyParser::ParseTypeOptionalitySpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyTypeOptionalitySpec* spec = new MyTypeOptionalitySpec();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "OPTIONAL")) {
		idx++;
		spec->Optional = true;
		success = true;
		goto done;
	}

	if (IsToken(idx, TOKEN_RESERVED_WORD, "DEFAULT")) {
		idx++;
		spec->Default = true;

		spec->Type = ParseType(idx);
		if (spec->Type == NULL) goto done;

		success = true;
	}

done:
	if (!success) {
		delete spec;
		return NULL;
	} else {
		tokIdx = idx;
		return spec;
	}
}
MyValueOptionalitySpec* MyParser::ParseValueOptionalitySpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyValueOptionalitySpec* spec = new MyValueOptionalitySpec();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "OPTIONAL")) {
		idx++;
		spec->Optional = true;
		success = true;
		goto done;
	}

	if (IsToken(idx, TOKEN_RESERVED_WORD, "DEFAULT")) {
		idx++;
		spec->Default = true;

		spec->Value = ParseValue(idx);
		if (spec->Value == NULL) goto done;

		success = true;
	}

done:
	if (!success) {
		delete spec;
		return NULL;
	} else {
		tokIdx = idx;
		return spec;
	}
}
MyValueSetOptionalitySpec* MyParser::ParseValueSetOptionalitySpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyValueSetOptionalitySpec* spec = new MyValueSetOptionalitySpec();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "OPTIONAL")) {
		idx++;
		spec->Optional = true;
		success = true;
		goto done;
	}

	if (IsToken(idx, TOKEN_RESERVED_WORD, "DEFAULT")) {
		idx++;
		spec->Default = true;

		spec->ValueSet = ParseValueSet(idx);
		if (spec->ValueSet == NULL) goto done;

		success = true;
	}

done:
	if (!success) {
		delete spec;
		return NULL;
	} else {
		tokIdx = idx;
		return spec;
	}
}
MyObjectOptionalitySpec* MyParser::ParseObjectOptionalitySpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjectOptionalitySpec* spec = new MyObjectOptionalitySpec();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "OPTIONAL")) {
		idx++;
		spec->Optional = true;
		success = true;
		goto done;
	}

	if (IsToken(idx, TOKEN_RESERVED_WORD, "DEFAULT")) {
		idx++;
		spec->Default = true;

		spec->Object = ParseObject(idx);
		if (spec->Object == NULL) goto done;

		success = true;
	}

done:
	if (!success) {
		delete spec;
		return NULL;
	} else {
		tokIdx = idx;
		return spec;
	}
}
MyObjectSetOptionalitySpec* MyParser::ParseObjectSetOptionalitySpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjectSetOptionalitySpec* spec = new MyObjectSetOptionalitySpec();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "OPTIONAL")) {
		idx++;
		spec->Optional = true;
		success = true;
		goto done;
	}

	if (IsToken(idx, TOKEN_RESERVED_WORD, "DEFAULT")) {
		idx++;
		spec->Default = true;

		spec->ObjectSet = ParseObjectSet(idx);
		if (spec->ObjectSet == NULL) goto done;

		success = true;
	}

done:
	if (!success) {
		delete spec;
		return NULL;
	} else {
		tokIdx = idx;
		return spec;
	}
}
MyFixedTypeValueFieldSpec* MyParser::ParseFixedTypeValueFieldSpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyFixedTypeValueFieldSpec* spec = new MyFixedTypeValueFieldSpec();

	if (!ParseValueFieldReference(idx, &spec->ValueFieldReference)) goto done;

	spec->Type = ParseType(idx);
	if (spec->Type == NULL) goto done;
	
	if (IsToken(idx, TOKEN_RESERVED_WORD, "UNIQUE")) {
		idx++;
		spec->Unique = true;
	}

	spec->ValueOptionalitySpec = ParseValueOptionalitySpec(idx); // Optional
	success = true;

done:
	if (!success) {
		delete spec;
		return NULL;
	} else {
		tokIdx = idx;
		return spec;
	}
}
MyVariableTypeValueFieldSpec* MyParser::ParseVariableTypeValueFieldSpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyVariableTypeValueFieldSpec* spec = new MyVariableTypeValueFieldSpec();

	if (!ParseValueFieldReference(idx, &spec->ValueFieldReference)) goto done;

	spec->FieldName = ParseFieldName(idx);
	if (spec->FieldName == NULL) goto done;
	
	spec->ValueOptionalitySpec = ParseValueOptionalitySpec(idx); // Optional
	success = true;

done:
	if (!success) {
		delete spec;
		return NULL;
	} else {
		tokIdx = idx;
		return spec;
	}
}
MyFixedTypeValueSetFieldSpec* MyParser::ParseFixedTypeValueSetFieldSpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyFixedTypeValueSetFieldSpec* spec = new MyFixedTypeValueSetFieldSpec();

	if (!ParseValueSetFieldReference(idx, &spec->ValueSetFieldReference)) goto done;

	spec->Type = ParseType(idx);
	if (spec->Type == NULL) goto done;
	
	spec->ValueSetOptionalitySpec = ParseValueSetOptionalitySpec(idx); // Optional
	success = true;

done:
	if (!success) {
		delete spec;
		return NULL;
	} else {
		tokIdx = idx;
		return spec;
	}
}
MyVariableTypeValueSetFieldSpec* MyParser::ParseVariableTypeValueSetFieldSpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyVariableTypeValueSetFieldSpec* spec = new MyVariableTypeValueSetFieldSpec();

	if (!ParseValueSetFieldReference(idx, &spec->ValueSetFieldReference)) goto done;

	spec->FieldName = ParseFieldName(idx);
	if (spec->FieldName == NULL) goto done;
	
	spec->ValueSetOptionalitySpec = ParseValueSetOptionalitySpec(idx); // Optional
	success = true;

done:
	if (!success) {
		delete spec;
		return NULL;
	} else {
		tokIdx = idx;
		return spec;
	}

}
MyObjectFieldSpec* MyParser::ParseObjectFieldSpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjectFieldSpec* spec = new MyObjectFieldSpec();

	if (!ParseObjectFieldReference(idx, &spec->ObjectFieldReference)) goto done;

	spec->DefinedObjectClass = ParseDefinedObjectClass(idx);
	if (spec->DefinedObjectClass == NULL) goto done;
	
	spec->ObjectOptionalitySpec = ParseObjectOptionalitySpec(idx); // Optional
	success = true;

done:
	if (!success) {
		delete spec;
		return NULL;
	} else {
		tokIdx = idx;
		return spec;
	}
}
MyObjectSetFieldSpec* MyParser::ParseObjectSetFieldSpec(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyObjectSetFieldSpec* spec = new MyObjectSetFieldSpec();

	if (!ParseObjectSetFieldReference(idx, &spec->ObjectSetFieldReference)) goto done;

	spec->DefinedObjectClass = ParseDefinedObjectClass(idx);
	if (spec->DefinedObjectClass == NULL) goto done;
	
	spec->ObjectSetOptionalitySpec = ParseObjectSetOptionalitySpec(idx); // Optional
	success = true;

done:
	if (!success) {
		delete spec;
		return NULL;
	} else {
		tokIdx = idx;
		return spec;
	}
}
bool MyParser::ParseValueFieldReference(int& tokIdx, MyStringA* ret) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "&")) goto done;
	if (err = ExpectedTokenType(idx, TOKEN_VALUE_REF)) goto done;

	ret->Set(m_Tokens.Get(idx-1));
	success = true;

done:
	if (success) tokIdx = idx;
	return success;
}
bool MyParser::ParseTypeFieldReference(int& tokIdx, MyStringA* ret) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "&")) goto done;
	if (err = ExpectedTokenType(idx, TOKEN_TYPE_REF)) goto done;

	ret->Set(m_Tokens.Get(idx-1));
	success = true;

done:
	if (success) tokIdx = idx;
	return success;
}
bool MyParser::ParseValueSetFieldReference(int& tokIdx, MyStringA* ret) {
	return ParseTypeFieldReference(tokIdx, ret);
}
bool MyParser::ParseObjectFieldReference(int& tokIdx, MyStringA* ret) {
	return ParseTypeFieldReference(tokIdx, ret);
}
bool MyParser::ParseObjectSetFieldReference(int& tokIdx, MyStringA* ret) {
	return ParseTypeFieldReference(tokIdx, ret);
}

MyComponentTypeLists* MyParser::ParseComponentTypeLists(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyComponentTypeLists* lists = new MyComponentTypeLists();

	lists->RootComponentTypeList = ParseComponentTypeList(idx);
	if (lists->RootComponentTypeList) {

		if (!IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) {
			success = true;
			goto done;
		}
		idx++;

		lists->ExtensionAndException = ParseExtensionAndException(idx);
		if (lists->ExtensionAndException == NULL) goto done;

		lists->ExtensionAdditions = ParseExtensionAdditions(idx);
		if (lists->ExtensionAdditions == NULL) goto done;

		lists->OptionalExtensionMarker = ParseOptionalExtensionMarker(idx);
		if (lists->OptionalExtensionMarker) { success = true; goto done; }

		lists->ExtensionEndMarker = ParseExtensionEndMarker(idx);
		if (lists->ExtensionEndMarker == NULL) goto done;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) goto done;

		lists->RootComponentTypeList2 = ParseComponentTypeList(idx);
		if (lists->RootComponentTypeList2 == NULL) goto done;

		success = true;

	} else {

		lists->ExtensionAndException = ParseExtensionAndException(idx);
		if (lists->ExtensionAndException == NULL) goto done;

		lists->ExtensionAdditions = ParseExtensionAdditions(idx);
		if (lists->ExtensionAdditions == NULL) goto done;

		lists->OptionalExtensionMarker = ParseOptionalExtensionMarker(idx);
		if (lists->OptionalExtensionMarker) { success = true; goto done; }

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) goto done;

		lists->RootComponentTypeList2 = ParseComponentTypeList(idx);
		if (lists->RootComponentTypeList2 == NULL) goto done;

		success = true;
	}

done:
	if (!success) {
		delete lists;
		return NULL;
	} else {
		tokIdx = idx;
		return lists;
	}
}
MyComponentTypeList* MyParser::ParseComponentTypeList(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyComponentTypeList* list = new MyComponentTypeList();

	while (true) {
		MyComponentType* typ = ParseComponentType(idx);
		if (typ != NULL) {
			list->ComponentTypes.Add(typ);
			success = true;

			if (!IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) break;
			idx++;
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
MyComponentType* MyParser::ParseComponentType(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyComponentType* typ = new MyComponentType();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "COMPONENTS")) {
		idx++;

		if (err = ExpectedTokenType(idx, TOKEN_RESERVED_WORD, "OF")) goto done;

		typ->ComponentsOfType = ParseType(idx);
		if (typ->ComponentsOfType == NULL) goto done;
		success = true;
	} else {
		typ->NamedType = ParseNamedType(idx);
		if (typ->NamedType == NULL) goto done;

		if (IsToken(idx, TOKEN_RESERVED_WORD, "OPTIONAL")) {
			idx++;
			typ->Optional = true;
			success = true;
		} else if (IsToken(idx, TOKEN_RESERVED_WORD, "DEFAULT")) {
			idx++;
			typ->DefaultValue = ParseValue(idx);
			if (typ->DefaultValue == NULL) goto done;
			success = true;
		}
	}
	
done:
	if (!success) {
		delete typ;
		return NULL;
	} else {
		tokIdx = idx;
		return typ;
	}
}

MyTag* MyParser::ParseTag(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyTag* tag = new MyTag();

	if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, "[")) {
		idx++;

		tag->Class = ParseClass(idx);
		if (tag->Class == NULL) goto done;

		tag->ClassNumber = ParseClassNumber(idx);
		if (tag->ClassNumber == NULL) goto done;

		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "]")) goto done;

		success = true;
	}

done:
	if (!success) {
		delete tag;
		return NULL;
	} else {
		tokIdx = idx;
		return tag;
	}
}
MyClassNumber*  MyParser::ParseClassNumber(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyClassNumber* classNumber = new MyClassNumber();

	if (IsToken(idx, TOKEN_NUMBER)) {
		classNumber->Number.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
		goto done;
	}

	classNumber->DefinedValue = ParseDefinedValue(idx);
	if (classNumber->DefinedValue == NULL) goto done;
	success = true;

done:
	if (!success) {
		delete classNumber;
		return NULL;
	} else {
		tokIdx = idx;
		return classNumber;
	}
}
MyClass* MyParser::ParseClass(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyClass* c = new MyClass();

	if (IsToken(idx, TOKEN_RESERVED_WORD, "UNIVERSAL") || IsToken(idx, TOKEN_RESERVED_WORD, "APPLICATION") || IsToken(idx, TOKEN_RESERVED_WORD, "PRIVATE")) {
		c->Value.Set(m_Tokens.Get(idx));
		idx++;
	} else {
		// Empty
	}
	success = true;

done:
	if (!success) {
		delete c;
		return NULL;
	} else {
		tokIdx = idx;
		return c;
	}
}

bool MyParser::ParseObjectClassReference(int& tokIdx, MyStringA* ret) {
	if (IsToken(tokIdx, TOKEN_TYPE_REF)) {
		MyStringA* val = m_Tokens.Get(tokIdx);
		for (int i = 0; i < val->Length(); i++) {
			if (MyStringA::IsLowercaseLetter(val->CharAt(i))) return false;
		}
		ret->Set(val);
		return true;
	}
	return false;
}
MyReference* MyParser::ParseReference(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyReference* r = new MyReference();

	if (ParseObjectClassReference(idx, &r->ObjectClassReference)) {
		success = true;
		goto done;
	}

	if (IsToken(idx, TOKEN_TYPE_REF)) {
		r->TypeReference.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
		goto done;
	}

	if (IsToken(idx, TOKEN_VALUE_REF)) {
		r->ValueReference.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
		goto done;
	}

	// BUG
	if (IsToken(idx, TOKEN_VALUE_REF)) {
		r->ObjectReference.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
		goto done;
	}

	// BUG
	if (IsToken(idx, TOKEN_TYPE_REF)) {
		r->ObjectSetReference.Set(m_Tokens.Get(idx));
		idx++;
		success = true;
		goto done;
	}

done:
	if (!success) {
		delete r;
		return NULL;
	} else {
		tokIdx = idx;
		return r;
	}
}
MyGovernor* MyParser::ParseGovernor(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyGovernor* g = new MyGovernor();

	g->Type = ParseType(idx);
	if (g->Type) { success = true; goto done; }

	g->DefinedObjectClass = ParseDefinedObjectClass(idx);
	if (g->DefinedObjectClass) { success = true; goto done; }

done:
	if (!success) {
		delete g;
		return NULL;
	} else {
		tokIdx = idx;
		return g;
	}
}
MyParamGovernor* MyParser::ParseParamGovernor(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyParamGovernor* g = new MyParamGovernor();

	g->Governor = ParseGovernor(idx);
	if (g->Governor) { success = true; goto done; }

	g->DummyGovernor = ParseReference(idx);
	if (g->DummyGovernor) { success = true; goto done; }

done:
	if (!success) {
		delete g;
		return NULL;
	} else {
		tokIdx = idx;
		return g;
	}
}
MyParameter* MyParser::ParseParameter(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyParameter* p = new MyParameter();

	p->ParamGovernor = ParseParamGovernor(idx);
	if (p->ParamGovernor) {
		if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, ":")) goto done;
	}
	p->DummyReference = ParseReference(idx);
	if (p->DummyReference) { success = true; goto done; }

done:
	if (!success) {
		delete p;
		return NULL;
	} else {
		tokIdx = idx;
		return p;
	}
}
MyParameterList* MyParser::ParseParameterList(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyParameter* p = NULL;
	MyParameterList* list = new MyParameterList();

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) goto done;
	
	while (true) {
		p = ParseParameter(idx);
		if (p) {
			list->Parameters.Add(p);

			if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) {
				idx++;
			} else {
				break;
			}
		} else {
			break;
		}
	}
	
	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;

	success = true;

done:
	if (!success) {
		delete list;
		return NULL;
	} else {
		tokIdx = idx;
		return list;
	}
}

MySetting* MyParser::ParseSetting(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MySetting* s = new MySetting();

	s->Type = ParseType(idx);
	if (s->Type) { success = true; goto done; }

	s->Value = ParseValue(idx);
	if (s->Value) { success = true; goto done; }

	s->ValueSet = ParseValueSet(idx);
	if (s->ValueSet) { success = true; goto done; }

	s->Object = ParseObject(idx);
	if (s->Object) { success = true; goto done; }

	s->ObjectSet = ParseObjectSet(idx);
	if (s->ObjectSet) { success = true; goto done; }

done:
	if (!success) {
		delete s;
		return NULL;
	} else {
		tokIdx = idx;
		return s;
	}
}
MyFieldSetting* MyParser::ParseFieldSetting(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyFieldSetting* s = new MyFieldSetting();

	s->PrimitiveFieldName = ParsePrimitiveFieldName(idx);
	if (s->PrimitiveFieldName) { success = true; goto done; }

	s->Setting = ParseSetting(idx);
	if (s->Setting) { success = true; goto done; }

done:
	if (!success) {
		delete s;
		return NULL;
	} else {
		tokIdx = idx;
		return s;
	}
}
MyDefaultSyntax* MyParser::ParseDefaultSyntax(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyFieldSetting* fs = NULL;
	MyDefaultSyntax* s = new MyDefaultSyntax();

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) goto done;

	while (true) {
		fs = ParseFieldSetting(idx);
		
		if (fs == NULL) {
			break;
		}
		
		s->FieldSettings.Add(fs);

		if (IsToken(idx, TOKEN_SINGLE_CHAR_ITEM, ",")) {
			idx++;
		}
	}

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;
	success = true;

done:
	if (!success) {
		delete s;
		return NULL;
	} else {
		tokIdx = idx;
		return s;
	}
}
MyDefinedSyntaxToken* MyParser::ParseDefinedSyntaxToken(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyDefinedSyntaxToken* dst = new MyDefinedSyntaxToken();

	dst->Literal = ParseLiteral(idx);
	if (dst->Literal) { success = true; goto done; }

	dst->Setting = ParseSetting(idx);
	if (dst->Setting) { success = true; goto done; }

done:
	if (!success) {
		delete dst;
		return NULL;
	} else {
		tokIdx = idx;
		return dst;
	}
}
MyDefinedSyntax* MyParser::ParseDefinedSyntax(int& tokIdx) {
	int err = 0;
	bool success = false;
	int idx = tokIdx;
	MyDefinedSyntaxToken* dst = NULL;
	MyDefinedSyntax* s = new MyDefinedSyntax();

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "{")) goto done;

	while (true) {
		dst = ParseDefinedSyntaxToken(idx);
		
		if (dst == NULL) {
			break;
		}
		
		s->DefinedSyntaxTokens.Add(dst);
	}

	if (err = ExpectedTokenType(idx, TOKEN_SINGLE_CHAR_ITEM, "}")) goto done;
	success = true;

done:
	if (!success) {
		delete s;
		return NULL;
	} else {
		tokIdx = idx;
		return s;
	}
}

bool MyParser::IsToken(int tokIdx, int tokType, const char* token) {
	if (m_TokenTypes.Get(tokIdx) != tokType) return false;
	if (token && !m_Tokens.Get(tokIdx)->Equals(token)) return false;
	return true;
}
int MyParser::ExpectedTokenType(int& tokIdx, int tokType, const char* token) {
	int err = 0;
	if (m_TokenTypes.Get(tokIdx) != tokType) {
		return LastError(MY_ERR_INVALID_PARAMETERS, "Invalid token"); // TODO: more detail information
	}
	if (token && !m_Tokens.Get(tokIdx)->Equals(token)) {
		return LastError(MY_ERR_INVALID_PARAMETERS, "Invalid token"); // TODO: more detail information
	}
	tokIdx++;
	return 0;
}



#endif // _MY_PARSER_CPP_

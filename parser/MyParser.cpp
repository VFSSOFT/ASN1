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
	return 0;
}
MyObjectIDValue* MyParser::ParseObjectIDValue(int& tokIdx) {
	return 0;
}

MyAssignment* MyParser::ParseAssignment(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyAssignment* ass = new MyAssignment();


	return 0;
}
MyTypeAssignment* MyParser::ParseTypeAssignment(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyTypeAssignment* ass = new MyTypeAssignment();

	if (IsToken(idx, TOKEN_TYPE_REF)) {
		ass->TypeReference.Set(m_Tokens.Get(idx));
		idx++;

		
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


MyType* MyParser::ParseType(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyType* typ = new MyType();

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
	return 0;
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

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

	ass->TypeAssignment = ParseTypeAssignment(idx);
	if (ass->TypeAssignment) { success = true; goto done; }

	ass->ValueAssignment = ParseValueAssignment(idx);
	if (ass->ValueAssignment) { success = true; goto done; }

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

MyType* MyParser::ParseType(int& tokIdx) {
	bool success = false;
	int idx = tokIdx;
	MyType* typ = new MyType();

	typ->BuiltinType = ParseBuiltinType(idx);
	if (typ->BuiltinType != NULL) { success = true; goto done; }

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

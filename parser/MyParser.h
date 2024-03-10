#ifndef _MY_PARSER_H_
#define _MY_PARSER_H_

#include "MyTokenizer.h"
#include "../notations/MyModule.h"
#include "../notations/MyTypeValueDef.h"
#include "../notations/MyRelativeObjectIDType.h"
#include "../notations/MyEnumeratedTypeDef.h"
#include "../notations/MyBitstringTypeDef.h"
#include "../notations/MyBoolTypeDef.h"
#include "../notations/MyChoiceTypeDef.h"

class MyParser {
public:

	int Parse(const wchar_t* file);

private:
	MyModuleDef* ParseModuleDef(int& tokIdx);
	MyModuleID* ParseModuleIdentifier(int& tokIdx);
	MyTagDefault* ParseDefaultTag(int& tokIdx);
	MyExtensionDefault* ParseExtensionDefault(int& tokIdx);
	MyModuleBody* ParseModuleBody(int& tokIdx);
	MyExports* ParseExports(int& tokIdx);
	MyImports* ParseImports(int& tokIdx);
	MyAssignmentList* ParseAssignmentList(int& tokIdx);

	MyDefinitiveID* ParseDefinitiveIdentifier(int& tokIdx);
	MyDefinitiveObjIdComponentList* ParseDefinitiveObjIdComponentList(int& tokIdx);
	MyDefinitiveObjIdComponent* ParseDefinitiveObjIdComponent(int& tokIdx);
	MyNameForm* ParseNameForm(int& tokIdx);
	MyDefinitiveNumberForm* ParseDefinitiveNumberForm(int& tokIdx);
	MyDefinitiveNameAndNumberForm* ParseDefinitiveNameAndNumberForm(int& tokIdx);

	MySymbolsExported* ParseSymbolsExported(int& tokIdx);
	MySymbolList* ParseSymbolList(int& tokIdx);
	MySymbol* ParseSymbol(int& tokIdx);

	MySymbolsImported* ParseSymbolsImported(int& tokIdx);
	MySymbolsFromModuleList* ParseSymbolsFromModuleList(int& tokIdx);
	MySymbolsFromModule* ParseSymbolsFromModule(int& tokIdx);
	MyGlobalModuleReference* ParseGlobalModuleReference(int& tokIdx);
	MyAssignedID* ParseAssignedIdentifier(int& tokIdx);
	MyObjectIDValue* ParseObjectIDValue(int& tokIdx);

	MyAssignment* ParseAssignment(int& tokIdx);
	MyTypeAssignment* ParseTypeAssignment(int& tokIdx);

	MyType* ParseType(int& tokIdx);
	MyBuiltinType* ParseBuiltinType(int& tokIdx);
	MyBitStringType* ParseBitStringType(int& tokIdx);
	MyNamedBitList* ParseNamedBitList(int& tokIdx);
	MyNamedBit* ParseNamedBit(int& tokIdx);
	MyBooleanType* ParseBoolType(int& tokIdx);
	MyChoiceType* ParseChoiceType(int& tokIdx);
	MyAlternativeTypeLists* ParseAlternativeTypeLists(int& tokIdx);
	MyAlternativeTypeList* ParseAlternativeTypeList(int& tokIdx);
	MyExtensionAndException* ParseExtensionAndException(int& tokIdx);
	MyExtensionAdditionAlternative* ParseExtensionAdditionAlternative(int& tokIdx);
	MyOptionalExtensionMarker* ParseOptionalExtensionMarker(int& tokIdx);
	MyNamedType* ParseNamedType(int& tokIdx);
	MyExceptionSpec* ParseExceptionSpec(int& tokIdx);
	MyExceptionIdentification* ParseExceptionIdentification(int& tokIdx);
	MyExtensionAdditionAlternativeGroup* ParseExtensionAdditionAlternativeGroup(int& tokIdx);
	MyVersionNumber* ParseVersionNumber(int& tokIdx);

	MySignedNumber* ParseSignedNumber(int& tokIdx);
	MyDefinedValue* ParseDefinedValue(int& tokIdx);
	MyValue* ParseValue(int& tokIdx);

	bool IsToken(int tokIdx, int tokType, const char* token=NULL);
	int ExpectedTokenType(int tokIdx, int tokType, const char* token=NULL);

private:
	MyModuleDef*       m_ModuleDef;
	MyArray<MyStringA> m_Tokens;
	MyValArray<int>    m_TokenTypes;

	MY_LAST_ERROR_DECL;
};


#endif // _MY_PARSER_H_

#ifndef _MY_PARSER_H_
#define _MY_PARSER_H_

#include "MyTokenizer.h"
#include "../notations/MyModule.h"
#include "../notations/MyTypeValueDef.h"
#include "../notations/MyRelativeObjectIDType.h"
#include "../notations/MyEnumeratedTypeDef.h"

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

	bool IsToken(int tokIdx, int tokType, const char* token=NULL);
	int ExpectedTokenType(int tokIdx, int tokType, const char* token=NULL);

private:
	MyModuleDef*       m_ModuleDef;
	MyArray<MyStringA> m_Tokens;
	MyValArray<int>    m_TokenTypes;

	MY_LAST_ERROR_DECL;
};


#endif // _MY_PARSER_H_

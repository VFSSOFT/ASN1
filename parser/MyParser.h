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

	MyDefinitiveID* ParseDefinitiveIdentifier(int& tokIdx);
	MyDefinitiveObjIdComponentList* ParseDefinitiveObjIdComponentList(int& tokIdx);
	MyDefinitiveObjIdComponent* ParseDefinitiveObjIdComponent(int& tokIdx);
	MyNameForm* ParseNameForm(int& tokIdx);
	MyDefinitiveNumberForm* ParseDefinitiveNumberForm(int& tokIdx);
	MyDefinitiveNameAndNumberForm* ParseDefinitiveNameAndNumberForm(int& tokIdx);

	int ExpectedTokenType(int tokIdx, int tokType, const char* token=NULL);

private:
	MyModuleDef*       m_ModuleDef;
	MyArray<MyStringA> m_Tokens;
	MyValArray<int>    m_TokenTypes;

	MY_LAST_ERROR_DECL;
};


#endif // _MY_PARSER_H_

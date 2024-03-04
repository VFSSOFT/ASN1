#ifndef _MY_PARSER_H_
#define _MY_PARSER_H_

#include "MyTokenizer.h"
#include "../notations/MyModule.h"

class MyParser {
public:

	int Parse(const wchar_t* file);


private:
	int ParseModuleDef(MyTokenizer* tokenizer, MyModuleDef* moduleDef);
	int ParseModuleIdentifier(MyTokenizer* tokenizer, MyModuleID* moduleID);
	int ParseDefinitiveIdentifier(MyTokenizer* tokenizer, MyDefinitiveID* definitiveID);
	int ParseDefinitiveObjIdComponentList(MyTokenizer* tokenizer, MyDefinitiveObjIdComponentList* list);
	int ParseDefinitiveObjIdComponent(MyTokenizer* tokenizer, MyDefinitiveObjIdComponent* comp);
	int ParseNameForm(MyTokenizer* tokenizer, MyNameForm* nameForm);
	int ParseDefinitiveNumberForm(MyTokenizer* tokenizer, MyDefinitiveNumberForm* definitiveNumberForm);

	int ExpectedTokenType(MyTokenizer* tokenizer, int tokType);
	int InvalidToken(int expToken, int realToken, const char* defName=NULL);
	int InvalidToken(int expToken1, int expToken2, int realToken, const char* defName=NULL);

private:
	MyTokenizer m_Tokenizer;
	MyModuleDef m_ModuleDef;

	MY_LAST_ERROR_DECL;
};


#endif // _MY_PARSER_H_

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
	int ParseModuleDef(MyTokenizer* tokenizer, MyModuleDef* moduleDef);
	int ParseModuleIdentifier(MyTokenizer* tokenizer, MyModuleID* moduleID);
	int ParseDefinitiveIdentifier(MyTokenizer* tokenizer, MyDefinitiveID* definitiveID);
	int ParseDefinitiveObjIdComponentList(MyTokenizer* tokenizer, MyDefinitiveObjIdComponentList* list);
	int ParseDefinitiveObjIdComponent(MyTokenizer* tokenizer, MyDefinitiveObjIdComponent* comp);
	int ParseNameForm(MyTokenizer* tokenizer, MyNameForm* nameForm);
	int ParseDefinitiveNumberForm(MyTokenizer* tokenizer, MyDefinitiveNumberForm* definitiveNumberForm);
	int ParseTagDefaults(MyTokenizer* tokenizer, MyTagDefault* tagDefaults);
	int ParseModuleBody(MyTokenizer* tokenizer, MyModuleBody* body);
	int ParseExports(MyTokenizer* tokenizer, MyExports* exports);
	int ParseSymbolsExported(MyTokenizer* tokenizer, MySymbolsExported* exported);
	int ParseSymbolList(MyTokenizer* tokenizer, MySymbolList* symbolList);
	int ParseSymbol(MyTokenizer* tokenizer, MySymbol* symbol);
	int ParseReference(MyTokenizer* tokenizer, MyReference* reference);
	int ParseAssignList(MyTokenizer* tokenizer, MyAssignmentList* assignList);
	int ParseAssignment(MyTokenizer* tokenizer, MyAssignment* assignment);
	int ParseType(MyTokenizer* tokenizer, MyType* typ);
	int ParseBuiltinType(MyTokenizer* tokenizer, MyBuiltinType* builtinType);
	int ParseValue(MyTokenizer* tokenizer, MyValue* val);
	int ParseBuiltinValue(MyTokenizer* tokenizer, MyBuiltinValue* val);

	int Peek(MyTokenizer* tokenizer);
	int Next(MyTokenizer* tokenizer);
	int ExpectedTokenType(MyTokenizer* tokenizer, int tokType, const char* token=NULL);
	int ExpectedReservedWord(MyTokenizer* tokenizer, const char* token);
	int InvalidToken(int expToken, int realToken, const char* defName=NULL);
	int InvalidToken(int expToken1, int expToken2, int realToken, const char* defName=NULL);




private:
	MyTokenizer m_Tokenizer;
	MyModuleDef m_ModuleDef;

	MY_LAST_ERROR_DECL;
};


#endif // _MY_PARSER_H_

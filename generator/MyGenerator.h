#ifndef _MY_GENERATOR_H_
#define _MY_GENERATOR_H_

#include "../notations/MyModule.h"
#include "../notations/MyConstrainedTypeDef.h"
#include "../notations/MyCharacterStringType.h"
#include "../notations/MyTypeValueDef.h"
#include "../notations/MySequenceOfTypeDef.h"

#include "MyTypeInfo.h"
#include "MyValueInfo.h"
#include "MyAssignInfo.h"

class MyGenerator {
public:

	MyGenerator();
	virtual ~MyGenerator();

	int Process(MyModuleDef* modDef);
	int GenCode(MyStringA* ret);

private:
	int ProcessModuleId(MyModuleID* moduleId);
	int ProcessTagDefault(MyTagDefault* tagDefault);
	int ProcessAssignment(MyAssignment* ass);
	int ProcessValueAssignment(MyValueAssignment* valAss);
	int ProcessTypeAssignment(MyTypeAssignment* typAss);


	int ProcessType(MyType* typ, MyTypeInfo** retTypeInfo);
	int ProcessBuiltinType(MyBuiltinType* typ, MyTypeInfo** retTypeInfo);
	int ProcessReferenceType(MyReferencedType* typ, MyTypeInfo** retTypeInfo);
	int ProcessConstrainedType(MyConstrainedType* typ, MyTypeInfo** retTypeInfo);
	int ProcessDefinedType(MyDefinedType* typ, MyTypeInfo** retTypeInfo);
	int ProcessSequenceType(MySequenceType* typ, MyTypeInfo** retTypeInfo);
	int ProcessSequenceOfType(MySequenceOfType* typ, MyTypeInfo** retTypeInfo);
	int ProcessTaggedType(MyTaggedType* typ, MyTypeInfo** retTypeInfo);
	int ProcessUsefulType(MyStringA* typ, MyTypeInfo** retTypeInfo);

	int ProcessValue(MyValue* val, MyValueInfo** retValueInfo);
	int ProcessBuiltinValue(MyBuiltinValue* val, MyValueInfo** retValueInfo);

	int GenType(MyTypeAssignInfo* ass);
	const char* GenTypeStr(MyTypeInfo* ti);
private:

	MyStringA m_ModuleReference;
	MyStringA m_ModuleDefinitiveIdentifier;

	MyStringA m_DefaultTag;

	MyValArray<MyValueAssignInfo*> m_ValueAssigns;
	MyValArray<MyTypeAssignInfo*>  m_TypeAssigns;


	MyStringA m_OutputCode;
	MyStringA m_GenTypeStrBuf;

	MY_LAST_ERROR_DECL;
};





#endif // _MY_GENERATOR_H_


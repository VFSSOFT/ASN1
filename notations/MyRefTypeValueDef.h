#ifndef _MY_REF_TYPE_VALUE_DEF_H_
#define _MY_REF_TYPE_VALUE_DEF_H_

#include "MyNotationCommon.h"
#include "MyParameterizaionDef.h"


/*
ExternalTypeReference ::=
		modulereference
		"."
		typereference
*/
class MyExternalTypeReference : public NotationBase {
public:
	MyStringA ModuleReference;
	MyStringA TypeReference;
};

/*
ExternalValueReference ::=
		modulereference
		"."
		valuereference
*/
class MyExternalValueReference : public NotationBase {
public:
	MyStringA ModuleReference;
	MyStringA ValueReference;
};





/*
DefinedType ::=
		ExternalTypeReference
		| Typereference
		| ParameterizedType
		| ParameterizedValueSetType
*/
class MyDefinedType : public NotationBase {
public:
	MyExternalTypeReference     ExternalTypeReference;
	MyParameterizedType         ParameterizedType;
	MyParameterizedValueSetType ParameterizedValueSetType;
};




/*
DefinedValue ::=
		ExternalValueReference
		| Valuereference
		| ParameterizedValue
*/
class MyDefinedValue : public NotationBase {
public:
	MyExternalValueReference ExternalValueReference;
	MyParameterizedValue     ParameterizedValue;
};


#endif // _MY_REF_TYPE_VALUE_DEF_H_


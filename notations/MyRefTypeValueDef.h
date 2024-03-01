#ifndef _MY_REF_TYPE_VALUE_DEF_H_
#define _MY_REF_TYPE_VALUE_DEF_H_

#include "MyNotationCommon.h"


/*
ExternalTypeReference ::=
		modulereference
		"."
		typereference
*/
class MyExternalTypeReference : public NotationBase {
public:

};

/*
ExternalValueReference ::=
		modulereference
		"."
		valuereference
*/
class MyExternalValueReference : public NotationBase {

};

/*
SimpleDefinedType ::=
		ExternalTypeReference |
		typereference
*/
class MySimpleDefinedType : public NotationBase {
public:
	MyExternalTypeReference ExternalTypeReference;

};

/*
ActualParameter ::=
		Type
		| Value
		| ValueSet
		| DefinedObjectClass
		| Object
		| ObjectSet
*/
class MyActualParameter : public NotationBase {

};

/*
ActualParameterList ::=
		"{" ActualParameter "," + "}"
*/
class MyActualParameterList : public NotationBase {
public:
	MyActualParameter ActualParameters;
};

/*
ParameterizedType ::=
		SimpleDefinedType
		ActualParameterList
*/
class MyParameterizedType : public NotationBase {
public:
	MySimpleDefinedType   SimpleDefinedType;
	MyActualParameterList ActualParameterList;
};

/*
ParameterizedValueSetType ::=
		SimpleDefinedType
		ActualParameterList
*/
class MyParameterizedValueSetType : public NotationBase {
public:
	MySimpleDefinedType   SimpleDefinedType;
	MyActualParameterList ActualParameterList;
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
SimpleDefinedValue ::=
		ExternalValueReference |
		valuereference
*/
class MySimpleDefinedValue : public NotationBase {
public:
	MyExternalValueReference ExternalValueReference;
};

/*
ParameterizedValue ::=
		SimpleDefinedValue
		ActualParameterList
*/
class MyParameterizedValue : public NotationBase {
public:
	MySimpleDefinedValue  SimpleDefinedValue;
	MyActualParameterList ActualParameterList;
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


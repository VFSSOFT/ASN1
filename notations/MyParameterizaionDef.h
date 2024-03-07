#ifndef _MY_PARAMETRIZATION_DEF_H_
#define _MY_PARAMETRIZATION_DEF_H_

#include "MyNotationCommon.h"

class MyReference;
class MyExternalTypeReference;
class MyExternalValueReference;
class MyDefinedObjectClass;
class MyObject;
class MyObjectSet;
class MyDefinedObjectSet;
class MyObjectClass;
class MyType;
class MyValue;
class MyValueSet;

/*
DummyReference ::= Reference
*/
typedef MyReference MyDummyReference;

/*
DummyGovernor ::= DummyReference
*/
typedef MyDummyReference MyDummyGovernor;

/*
Governor ::= Type | DefinedObjectClass
*/
class MyGovernor : public NotationBase {
public:
	MyType*               Type;
	MyDefinedObjectClass* DefinedObjectClass;
};

/*
ParamGovernor ::= Governor | DummyGovernor
*/
class MyParamGovernor : public NotationBase {
public:
	MyGovernor*      Governor;
	MyDummyGovernor* DummyGovernor;
};

/*
Parameter ::= ParamGovernor ":" DummyReference | DummyReference
*/
class MyParameter : public NotationBase {
public:
	MyParamGovernor* ParamGovernor;
	MyDummyReference* DummyReference;
};


/*
ParameterList ::= "{" Parameter "," + "}"
*/
class MyParameterList : public NotationBase {
public:
	MyValArray<MyParameter*> Parameters;
};

/*
ParameterizedTypeAssignment ::=
		typereference
		ParameterList
		"::="
		Type
*/
class MyParameterizedTypeAssignment : public NotationBase {
public:
	MyStringA        TypeReference;
	MyParameterList* ParameterList;
	MyType*          Type;
};

/*
ParameterizedValueAssignment ::=
		valuereference
		ParameterList
		Type
		"::="
		Value
*/
class MyParameterizedValueAssignment : public NotationBase {
public:
	MyStringA        ValueReference;
	MyParameterList* ParameterList;
	MyType*          Type;
	MyValue*         Value;
};

/*
ParameterizedValueSetTypeAssignment ::=
		typereference
		ParameterList
		Type
		"::="
		ValueSet
*/
class MyParameterizedValueSetTypeAssignment : public NotationBase {
public:
	MyStringA        TypeReference;
	MyParameterList* ParameterList;
	MyType*          Type;
	MyValueSet*      ValueSet;
};

/*
ParameterizedObjectClassAssignment ::=
		objectclassreference
		ParameterList
		"::="
		ObjectClass
*/
class MyParameterizedObjectClassAssignment : public NotationBase {
public:
	MyStringA        ObjectClassReference;
	MyParameterList* ParameterList;
	MyObjectClass*   ObjectClass;
};

/*
ParameterizedObjectAssignment ::=
		objectreference
		ParameterList
		DefinedObjectClass
		"::="
		Object
*/
class MyParameterizedObjectAssignment : public NotationBase {
public:
	MyStringA             ObjectReference;
	MyParameterList*      ParameterList;
	MyDefinedObjectClass* DefinedObjectClass;
	MyObject*             Object;
};

/*
ParameterizedObjectSetAssignment ::=
		objectsetreference
		ParameterList
		DefinedObjectClass
		"::="
		ObjectSet
*/
class MyParameterizedObjectSetAssignment : public NotationBase {
public:
	MyStringA             ObjectSetReference;
	MyParameterList*      ParameterList;
	MyDefinedObjectClass* DefinedObjectClass;
	MyObjectSet*          ObjectSet;
};



/*
ParameterizedAssignment ::=
		ParameterizedTypeAssignment
		| ParameterizedValueAssignment
		| ParameterizedValueSetTypeAssignment
		| ParameterizedObjectClassAssignment
		| ParameterizedObjectAssignment
		| ParameterizedObjectSetAssignment
*/
class MyParameterizedAssignment : public NotationBase {
public:
	MyParameterizedTypeAssignment*         ParameterizedTypeAssignment;
	MyParameterizedValueAssignment*        ParameterizedValueAssignment;
	MyParameterizedValueSetTypeAssignment* ParameterizedValueSetTypeAssignment;
	MyParameterizedObjectClassAssignment*  ParameterizedObjectClassAssignment;
	MyParameterizedObjectAssignment*       ParameterizedObjectAssignment;
	MyParameterizedObjectSetAssignment*    ParameterizedObjectSetAssignment;
};


/*
ParameterizedReference ::= Reference | Reference "{" "}"
*/
class MyParameterizedReference : public NotationBase {
public:
	MyReference* Reference;
};


/*
SimpleDefinedType ::=
		ExternalTypeReference |
		typereference
*/
class MySimpleDefinedType : public NotationBase {
public:
	MyExternalTypeReference* ExternalTypeReference;
	MyStringA                TypeReference;
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
public:
	MyType*               Type;
	MyValue*              Value;
	MyValueSet*           ValueSet;
	MyDefinedObjectClass* DefinedObjectClass;
	MyObject*             Object;
	MyObjectSet*          ObjectSet;
};

/*
ActualParameterList ::=
		"{" ActualParameter "," + "}"
*/
class MyActualParameterList : public NotationBase {
public:
	MyValArray<MyActualParameter*> ActualParameters;
};


/*
ParameterizedType ::=
		SimpleDefinedType
		ActualParameterList
*/
class MyParameterizedType : public NotationBase {
public:
	MySimpleDefinedType*   SimpleDefinedType;
	MyActualParameterList* ActualParameterList;
};


/*
ParameterizedValueSetType ::=
		SimpleDefinedType
		ActualParameterList
*/
class MyParameterizedValueSetType : public NotationBase {
public:
	MySimpleDefinedType*   SimpleDefinedType;
	MyActualParameterList* ActualParameterList;
};


/*
SimpleDefinedValue ::=
		ExternalValueReference |
		valuereference
*/
class MySimpleDefinedValue : public NotationBase {
public:
	MyExternalValueReference* ExternalValueReference;
};


/*
ParameterizedObjectClass ::=
		DefinedObjectClass
		ActualParameterList
*/
class MyParameterizedObjectClass : public NotationBase {
public:
	MyDefinedObjectClass*  DefinedObjectClass;
	MyActualParameterList* ActualParameterList;
};

/*
ParameterizedObjectSet ::=
		DefinedObjectSet
		ActualParameterList
*/
class MyParameterizedObjectSet : public NotationBase {
public:
	MyDefinedObjectSet*    DefinedObjectSet;
	MyActualParameterList* ActualParameterList;
};

/*
ParameterizedValue ::=
		SimpleDefinedValue
		ActualParameterList
*/
class MyParameterizedValue : public NotationBase {
public:
	MySimpleDefinedValue*  SimpleDefinedValue;
	MyActualParameterList* ActualParameterList;
};




#endif // _MY_PARAMETRIZATION_DEF_H_



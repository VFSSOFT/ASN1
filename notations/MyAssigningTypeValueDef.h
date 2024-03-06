#ifndef MY_ASSIGNING_TYPE_VALUE_DEF_H_
#define MY_ASSIGNING_TYPE_VALUE_DEF_H_

#include "MyNotationCommon.h"
#include "MyTypeValueDef.h"

/*
TypeAssignment ::=
		typereference
		"::="
		Type
*/
class MyTypeAssignment: public NotationBase {
public:
	MyStringA TypeReference;
	MyType    Type;
};

/*
ValueAssignment ::=
		valuereference
		Type
		"::="
		Value
*/
class MyValueAssignment : public NotationBase {
public:
	MyStringA ValueReference;
	MyType    Type;
	MyValue   Value;
};


/*
XMLTypedValue ::=
		"<" & NonParameterizedTypeName ">"
		XMLValue
		"</" & NonParameterizedTypeName ">"
		| "<" & NonParameterizedTypeName "/
*/
class MyXMLTypedValue : public NotationBase {
public:

};


/*
XMLValueAssignment ::=
		valuereference
		"::="
		XMLTypedValue
*/
class MyXMLValueAssignment : public NotationBase {
public:
	MyStringA       ValueReference;
	MyXMLTypedValue XMLTypedValue;
};

/*
ValueSet ::= "{" ElementSetSpecs "}"
*/
class MyValueSet : public NotationBase {
public:
};

/*
ValueSetTypeAssignment ::=
		typereference
		Type
		"::="
		ValueSet
*/
class MyValueSetTypeAssignment : public NotationBase {
public:
	MyStringA   TypeReference;
	MyType      Type;
	MyValueSet  ValueSet;

};

#endif // MY_ASSIGNING_TYPE_VALUE_DEF_H_


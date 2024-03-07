#ifndef _MY_ENUMERATED_TYPE_DEF_H_
#define _MY_ENUMERATED_TYPE_DEF_H_

#include "MyNotationCommon.h"
#include "MyIntegerTypeDef.h"

/*
EnumerationItem ::= identifier | NamedNumber
*/
class MyEnumerationItem : public NotationBase {
public:
	MyStringA      Identifier;
	MyNamedNumber* NamedNumber;
};

/*
Enumeration ::= EnumerationItem | EnumerationItem "," Enumeration
*/
class MyEnumeration : public NotationBase {
public:
	MyValArray<MyEnumerationItem*> EnumerationItems;
};

/*
AdditionalEnumeration ::= Enumeration
*/
typedef MyEnumeration MyAdditionalEnumeration;

/*
RootEnumeration ::= Enumeration
*/
typedef MyEnumeration MyRootEnumeration;

/*
Enumerations ::=
		RootEnumeration
		| RootEnumeration "," "..." ExceptionSpec
		| RootEnumeration "," "..." ExceptionSpec "," AdditionalEnumeration
*/
class MyEnumerations : public NotationBase {
public:
	MyRootEnumeration*       RootEnumeration;
	MyAdditionalEnumeration* AdditionalEnumeration;
};

/*
EnumeratedType ::=
		ENUMERATED "{" Enumerations "}"
*/
class MyEnumeratedType : public NotationBase {
public:
	MyEnumerations* Enumerations;
};

/*
EnumeratedValue ::= identifier
*/
class MyEnumeratedValue : public NotationBase {
public:
	MyStringA Identifier;
};

/*
XMLEnumeratedValue ::= "<" & identifier "/>"
*/
class MyXMLEnumeratedValue : public NotationBase {
public:
	MyStringA Identifier;
};

#endif // _MY_ENUMERATED_TYPE_DEF_H_

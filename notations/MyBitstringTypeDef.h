#ifndef _MY_BITSTRING_TYPE_DEF_H_
#define _MY_BITSTRING_TYPE_DEF_H_

#include "MyNotationCommon.h"
#include "MyRefTypeValueDef.h"
#include "MyTypeValueDef.h"

/*
NamedBit ::=
		identifier "(" number ")"
		| identifier "(" DefinedValue ")"
*/
class MyNamedBit : public NotationBase {
public:
	MyStringA      Identifier;
	MyStringA      Number;
	MyDefinedValue DefinedValue;
};

/*
NamedBitList ::=
		NamedBit
		| NamedBitList "," NamedBit
*/
class MyNamedBitList : public NotationBase {
public:
	MyArray<MyNamedBit> NamedBits;
};

/*
BitStringType ::=
		BIT STRING
		| BIT STRING "{" NamedBitList "}"
*/
class MyBitStringType : public NotationBase {
public:
	MyNamedBitList NamedBitList;
};

class MyIdentifierList : public NotationBase {
public:
	MyArray<MyStringA> Identifiers;
};

/*
BitStringValue ::=
		bstring
		| hstring
		| "{" IdentifierList "}"
		| "{" "}"
		| CONTAINING Value
*/
class MyBitStringValue : public NotationBase {
public:
	MyStringA        BString;
	MyStringA        HString;
	MyIdentifierList IdentifierList;
	MyValue          ContainingValue;
};


class MyXMLIdentifierList : public NotationBase {
public:
	MyArray<MyStringA> Identifiers;
};

/*
XMLBitStringValue ::=
		XMLTypedValue
		| Xmlbstring
		| XMLIdentifierList
		| empty
*/
class MyXMLBitStringValue : public NotationBase {
public:
	MyStringA           XmlBString;
	MyXMLIdentifierList XMLIdentifierList;
	bool                Empty;
};


#endif // _MY_BITSTRING_TYPE_DEF_H_


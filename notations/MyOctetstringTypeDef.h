#ifndef _MY_OCTETSTRING_TYPE_DEF_H_
#define _MY_OCTETSTRING_TYPE_DEF_H_


#include "MyNotationCommon.h"
#include "MyTypeValueDef.h"

/*
OctetStringType ::= OCTET STRING
*/
class MyOctetStringType : public NotationBase {};

/*
OctetStringValue ::=
		bstring
		| hstring
		| CONTAINING Value
*/
class MyOctetStringValue : public NotationBase {
public:
	MyStringA  BString;
	MyStringA  HString;
	MyValue*   ContainingValue;
};

/*
XMLOctetStringValue ::=
		XMLTypedValue
		| xmlhstring
*/
class MyXMLOctetStringValue : public NotationBase {
public:

};


#endif // _MY_OCTETSTRING_TYPE_DEF_H_

#ifndef _MY_TAGGED_TYPE_DEF_H_
#define _MY_TAGGED_TYPE_DEF_H_

#include "MyNotationCommon.h"
#include "MyTypeValueDef.h"


/*
ClassNumber ::=
		number
		| DefinedValue
*/
class MyClassNumber : public NotationBase {
public: 
	MyStringA       Number;
	MyDefinedValue* DefinedValue;
};

/*
Class ::=
		UNIVERSAL
		| APPLICATION
		| PRIVATE
		| empty
*/
class MyClass : public NotationBase {
public:
	MyStringA Value;
};

/*
Tag ::= "[" Class ClassNumber "]"
*/
class MyTag : public NotationBase {
public:
	MyClass*       Class;
	MyClassNumber* ClassNumber;
};

/*
TaggedType ::=
		Tag Type
		| Tag IMPLICIT Type
		| Tag EXPLICIT Type
*/
class MyTaggedType : public NotationBase {
public:
	MyTag*     Tag;
	MyType*    Type;
	MyStringA ImplicitOrExplicit;
};


typedef MyValue MyTaggedValue;
typedef MyXMLValue MyXMLTaggedValue;

#endif // _MY_TAGGED_TYPE_DEF_H_
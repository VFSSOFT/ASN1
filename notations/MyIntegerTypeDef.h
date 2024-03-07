#ifndef _MY_INTERGER_TYPE_DEF_H_
#define _MY_INTERGER_TYPE_DEF_H_

#include "MyNotationCommon.h"
#include "MyRefTypeValueDef.h"

/*
SignedNumber ::=
		number
		| "-" number
*/
class MySignedNumber : public NotationBase {
public:
	bool      Negative;
	MyStringA Number;
};

/*
NamedNumber ::=
		identifier "(" SignedNumber ")"
		| identifier "(" DefinedValue ")
*/
class MyNamedNumber : public NotationBase {
public:
	MyStringA       Identifier;
	MySignedNumber* SignedNumber;
	MyDefinedValue* DefinedValue;
};

/*
NamedNumberList ::=
		NamedNumber
		| NamedNumberList "," NamedNumber
*/
class MyNamedNumberList : public NotationBase {
public:
	MyValArray<MyNamedNumber*> NamedNumbers;
};

/*
IntegerType ::=
		INTEGER
		| INTEGER "{" NamedNumberList "}"
*/
class MyIntegerType : public NotationBase {
public:
	MyNamedNumberList* NamedNumberList;
};


/*
IntegerValue ::=
		SignedNumber
		| identifier
*/
class MyIntegerValue : public NotationBase {
public:
	MySignedNumber* SignedNumber;
	MyStringA       Identifier;
};


/*
XMLIntegerValue ::=
		SignedNumber
		| "<" & identifier "/>"
*/
class MyXMLIntegerValue : public NotationBase {
public:
	MySignedNumber* SignedNumber;
	MyStringA       Identifier;
};


#endif // _MY_INTERGER_TYPE_DEF_H_

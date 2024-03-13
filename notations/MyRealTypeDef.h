#ifndef _MY_REAL_TYPE_DEF_H_
#define _MY_REAL_TYPE_DEF_H_

#include "MyNotationCommon.h"

/*
RealType ::= REAL
*/
class MyRealType : public NotationBase { };


/*
NumericRealValue ::=
		realnumber
		| "-" realnumber
		| SequenceValue -- Value of the associated sequence type
*/
class MyNumericRealValue : public NotationBase {
public:
	bool             Negative;
	MyStringA        RealNumber;
	MySequenceValue* SequenceValue;
};

/*
SpecialRealValue ::=
		PLUS-INFINITY
		| MINUS-INFINITY
*/
class MySpecialRealValue : public NotationBase {
public:
	MyStringA Value;
};

/*
RealValue ::=
		NumericRealValue
		| SpecialRealValue
*/
class MyRealValue : public NotationBase {
public:
	MyNumericRealValue* NumericValue;
	MySpecialRealValue* SpecialValue;
};

/*
XMLNumericRealValue ::=
		realnumber
		| "-" realnumber
*/
class MyXMLNumericRealValue : public NotationBase {
public:
	bool      Negative;
	MyStringA RealNumber;
};

/*
XMLSpecialRealValue ::=
		"<" & PLUS-INFINITY "/>" | "<" & MINUS-INFINITY "/>"
*/
class MyXMLSpecialRealValue : public NotationBase {

};


/*
XMLRealValue ::=
		XMLNumericRealValue | XMLSpecialRealValue
*/
class MyXMLRealValue : public NotationBase {
public:
	MyXMLNumericRealValue* NumericRealValue;
	MyXMLSpecialRealValue* SpecialRealValue;
};



#endif // _MY_REAL_TYPE_DEF_H_

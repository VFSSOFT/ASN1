#ifndef _MY_BOOL_TYPE_DEF_H_
#define _MY_BOOL_TYPE_DEF_H_

#include "MyNotationCommon.h"

/*
BooleanType ::= BOOLEAN
*/
class MyBooleanType : public NotationBase { };

/*
* BooleanValue ::= TRUE | FALSE
*/
class MyBooleanValue : public NotationBase {
public:
	bool Value;
};

/*
XMLBooleanValue ::=
		"<" & "true" "/>"
		| "<" & "false" "/>"
*/
class MyXMLBooleanValue : public NotationBase {
public:
	bool Value;
};





#endif // _MY_BOOL_TYPE_DEF_H_


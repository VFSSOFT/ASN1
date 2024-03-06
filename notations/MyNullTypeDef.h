#ifndef _MY_NULL_TYPE_DEF_H_
#define _MY_NULL_TYPE_DEF_H_

#include "MyNotationCommon.h"

/*
NullType ::= NULL
*/
class MyNullType: public NotationBase {};


/*
NullValue ::= NULL
*/
class MyNullValue : public NotationBase {};

/*
XMLNullValue ::= empty
*/
class MyXMLNullValue: public NotationBase {};

#endif // _MY_NULL_TYPE_DEF_H_

#ifndef _MY_EXTERNAL_TYPE_H_
#define _MY_EXTERNAL_TYPE_H_

#include "MyNotationCommon.h"
#include "MySequenceTypeDef.h"

/*
ExternalType ::= EXTERNAL
*/
class MyExternalType: public NotationBase {};


typedef MySequenceValue MyExternalValue;
typedef MyXMLSequenceValue MyXMLExternalValue;

#endif // _MY_EXTERNAL_TYPE_H_



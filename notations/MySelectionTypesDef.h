#ifndef _MY_SELECTION_TYPE_DEF_H_
#define _MY_SELECTION_TYPE_DEF_H_

#include "MyNotationCommon.h"
#include "MyTypeValueDef.h"

/*
SelectionType ::= identifier "<" Type
*/
class MySelectionType : public NotationBase {
public:
	MyStringA Identifier;
	MyType    Type;
};

#endif // _MY_SELECTION_TYPE_DEF_H_


#ifndef _MY_SET_OF_TYPE_DEF_H_
#define _MY_SET_OF_TYPE_DEF_H_

#include "MyNotationCommon.h"
#include "MyTypeValueDef.h"
#include "MySequenceTypeDef.h"
#include "MySequenceOfTypeDef.h"

/*
SetOfType ::=
		SET OF Type
		| SET OF NamedType
*/
class MySetOfType : public NotationBase {
public:
	MyType      Type;
	MyNamedType NamedType;
};


/*
SetOfValue ::=
		"{" ValueList "}"
		| "{" NamedValueList "}"
		| "{" "}"
*/
class MySetOfValue : public NotationBase {
public:
	MyValueList ValueList;
	MyNamedValueList NamedValueList;
};

/*
XMLSetOfValue ::=
		XMLValueList
		| XMLDelimitedItemList
		| XMLSpaceSeparatedList
		| empty
*/
class MyXMLSetOfValue : public NotationBase {

};

#endif // _MY_SET_OF_TYPE_DEF_H_

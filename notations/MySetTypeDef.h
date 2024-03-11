#ifndef _MY_SET_TYPE_DEF_H_
#define _MY_SET_TYPE_DEF_H_

#include "MyNotationCommon.h"
#include "MySequenceTypeDef.h"

/*
SetType ::=
		SET "{" "}"
		| SET "{" ExtensionAndException OptionalExtensionMarker "}"
		| SET "{" ComponentTypeLists "}"
*/
class MySetType : public NotationBase {
public:
	MyExtensionAndException*   ExtensionAndException;
	MyOptionalExtensionMarker* OptionalExtensionMarker;
	MyComponentTypeLists*      ComponentTypeLists;
};

/*
SetValue ::=
		"{" ComponentValueList "}"
		| "{" "}"
*/
class MySetValue : public NotationBase {
public:
	MyComponentValueList* ComponentValueList;
};


/*
XMLSetValue ::=
		XMLComponentValueList
		| empty
*/
class MyXMLSetValue :public NotationBase {
public:
	MyXMLComponentValueList* XMLComponentValueList;
	bool                     Empty;
};




#endif // _MY_SET_TYPE_DEF_H_



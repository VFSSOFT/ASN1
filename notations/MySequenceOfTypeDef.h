#ifndef _MY_SEQUENCE_OF_TYPE_DEF_H_
#define _MY_SEQUENCE_OF_TYPE_DEF_H_

#include "MyNotationCommon.h"
#include "MyTypeValueDef.h"


/*
SequenceOfType ::= SEQUENCE OF Type | SEQUENCE OF NamedType
*/
class MySequenceOfType : public NotationBase {
public:
	MyType      Type;
	MyNamedType NamedType;
};


/*
ValueList ::=
		Value
		| ValueList "," Value
*/
class MyValueList : public NotationBase {
public:
	MyArray<MyValue> Values;
};

/*
NamedValueList ::=
		NamedValue
		| NamedValueList "," NamedValue
*/
class MyNamedValueList : public NotationBase {
public:
	MyArray<MyNamedValue> NamedValues;
};


/*
SequenceOfValue ::=
		"{" ValueList "}"
		| "{" NamedValueList "}"
		| "{" "}"
*/
class MySequenceOfValue : public NotationBase {
public:
	MyValueList      ValueList;
	MyNamedValueList NamedValueList;
};


/*
XMLValueList ::=
		XMLValueOrEmpty
		| XMLValueOrEmpty XMLValueList
*/
class MyXMLValueOrEmpty : public NotationBase {

};

/*
XMLValueList ::=
		XMLValueOrEmpty
		| XMLValueOrEmpty XMLValueList
*/
class MyXMLValueList : public NotationBase {

};

/*
XMLSequenceOfValue ::=
		XMLValueList
		| XMLDelimitedItemList
		| XMLSpaceSeparatedList
		| empty
*/
class MyXMLSequenceOfValue : public NotationBase {

};


#endif // _MY_SEQUENCE_OF_TYPE_DEF_H_


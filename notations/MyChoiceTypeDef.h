#ifndef _MY_CHOCE_TYPE_DEF_H_
#define _MY_CHOCE_TYPE_DEF_H_

#include "MyNotationCommon.h"
#include "MyTypeValueDef.h"
#include "MySequenceTypeDef.h"


/*
AlternativeTypeList ::=
		NamedType
		| AlternativeTypeList "," NamedType
*/
class MyAlternativeTypeList : public NotationBase{
public:
	MyValArray<MyNamedType*> NamedTypes;
};

typedef MyAlternativeTypeList MyRootAlternativeTypeList;


/*
ExtensionAdditionAlternativesGroup ::=
		"[[" VersionNumber AlternativeTypeList "]]"
*/
class MyExtensionAdditionAlternativeGroup : public NotationBase {
public:
	MyVersionNumber*       VersionNumber;
	MyAlternativeTypeList* AlternativeTypeList;
};

/*
ExtensionAdditionAlternative ::=
		ExtensionAdditionAlternativesGroup
		| NamedType
*/
class MyExtensionAdditionAlternative : public NotationBase {
public:
	MyExtensionAdditionAlternativeGroup* ExtensionAdditionAlternativeGroup;
	MyNamedType*                         NamedType;
};

/*
ExtensionAdditionAlternativesList ::=
		ExtensionAdditionAlternative
		| ExtensionAdditionAlternativesList "," ExtensionAdditionAlternative
*/
class MyExtensionAdditionAlternativesList : public NotationBase {
public:
	MyValArray<MyExtensionAdditionAlternative*> ExtensionAdditionAlternatives;
};


/*
ExtensionAdditionAlternatives ::=
		"," ExtensionAdditionAlternativesList
		| empty
*/
class MyExtensionAdditionAlternatives : public NotationBase {
public:
	MyExtensionAdditionAlternativesList* ExtensionAdditionAlternativesList;
};

/*
AlternativeTypeLists ::=
		RootAlternativeTypeList
		| RootAlternativeTypeList "," ExtensionAndException ExtensionAdditionAlternatives OptionalExtensionMarker
*/
class MyAlternativeTypeLists : public NotationBase {
public:
	MyRootAlternativeTypeList*      RootAlternativeTypeList;
	MyExtensionAndException*        ExtensionAndException;
	MyExtensionAdditionAlternative* ExtensionAdditionAlternatives;
	MyOptionalExtensionMarker*      OptionalExtensionMarker;
};

/*
ChoiceType ::= CHOICE "{" AlternativeTypeLists "}"
*/
class MyChoiceType : public NotationBase {
public:
	MyAlternativeTypeLists* AlternativeTypeLists;
};

/*
ChoiceValue ::= identifier ":" Value
*/
class MyChoiceValue : public NotationBase {
public:
	MyStringA  Identifier;
	MyValue*   Value;
};

/*
XMLChoiceValue ::= "<" & identifier ">" XMLValue "</" & identifier ">"
*/
class MyXMLChoiceValue : public NotationBase {
public:
	MyStringA   Identifier;
	MyXMLValue* Value;
};

#endif // _MY_CHOCE_TYPE_DEF_H_


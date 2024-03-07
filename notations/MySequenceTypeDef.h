#ifndef _MY_SEQUENCE_TYPE_DEF_H_
#define _MY_SEQUENCE_TYPE_DEF_H_

#include "MyNotationCommon.h"
#include "MyTypeValueDef.h"


/*
ExtensionAndException ::= "..." | "..." ExceptionSpec
*/
class MyExtensionAndException : public NotationBase {

};

/*
OptionalExtensionMarker ::= "," "..." | empty
*/
class MyOptionalExtensionMarker : public NotationBase {
public:

};

/*
ExtensionEndMarker ::= "," "..."
*/
class MyExtensionEndMarker : public NotationBase {

};

/*
VersionNumber ::= empty | number ":"
*/
class MyVersionNumber : public NotationBase {
public:
	MyStringA Number;
};

/*
ComponentType ::=
		NamedType
		| NamedType OPTIONAL
		| NamedType DEFAULT Value
		| COMPONENTS OF Type
*/
class MyComponentType : public NotationBase {
public:
	MyNamedType* NamedType;
	MyNamedType* OptionalNamedType;
	MyValue*     DefaultValue;
	MyType*      ComponentsOfType;
};

/*
ComponentTypeList ::=
		ComponentType
		| ComponentTypeList "," ComponentType
*/
class MyComponentTypeList : public NotationBase {
public:
	MyValArray<MyComponentType*> ComponentTypes;
};

/*
RootComponentTypeList ::= ComponentTypeList
*/
typedef MyComponentTypeList MyRootComponentTypeList;

/*
ExtensionAdditionGroup ::= "[[" VersionNumber ComponentTypeList "]]"
*/
class MyExtensionAdditionGroup : public NotationBase {
public:
	MyVersionNumber*     VersionNumber;
	MyComponentTypeList* ComponentTypeList;
};

/*
ExtensionAddition ::=
		ComponentType
		| ExtensionAdditionGroup
*/
class MyExtensionAddition : public NotationBase {
public:
	MyComponentType*          ComponentType;
	MyExtensionAdditionGroup* ExtensionAdditionGroup;
};

/*
ExtensionAdditionList ::=
		ExtensionAddition
		| ExtensionAdditionList "," ExtensionAddition
*/
class MyExtensionAdditionList : public NotationBase {
public:
	MyValArray<MyExtensionAddition*> ExtensionAdditions;
};

/*
ExtensionAdditions ::=
		"," ExtensionAdditionList
		| empty
*/
class MyExtensionAdditions : public NotationBase {
public:
	MyExtensionAdditionList* ExtensionAdditionList;
	bool                     Empty;
};


/*
ComponentTypeLists ::=
		RootComponentTypeList
		| RootComponentTypeList "," ExtensionAndException ExtensionAdditions OptionalExtensionMarker
		| RootComponentTypeList "," ExtensionAndException ExtensionAdditions ExtensionEndMarker "," RootComponentTypeList
		| ExtensionAndException ExtensionAdditions ExtensionEndMarker "," RootComponentTypeList
		| ExtensionAndException ExtensionAdditions OptionalExtensionMarker
*/
class MyComponentTypeLists : public NotationBase {
public:
	MyRootComponentTypeList* RootComponentTypeList;
	MyExtensionAndException* ExtensionAndException;
	MyExtensionAdditions*    ExtensionAdditions;
	MyRootComponentTypeList* RootComponentTypeList2;
};

/*
SequenceType ::=
		SEQUENCE "{" "}"
		| SEQUENCE "{" ExtensionAndException OptionalExtensionMarker "}"
		| SEQUENCE "{" ComponentTypeLists "}"
*/
class MySequenceType : public NotationBase {
public:
	MyExtensionAndException* ExtensionAndException;
	MyComponentTypeLists*    ComponentTypeLists;
};


/*
ComponentValueList ::=
		NamedValue
		| ComponentValueList "," NamedValue
*/
class MyComponentValueList : public NotationBase {
public:
	MyValArray<MyNamedValue*> NamedValues;
};

/*
SequenceValue ::=
		"{" ComponentValueList "}"
		| "{" "}"
*/
class MySequenceValue : public NotationBase {
public:
	MyComponentValueList* ComponentValueList;
};

/*
XMLComponentValueList ::=
		XMLNamedValue
		| XMLComponentValueList XMLNamedValue
*/
class MyXMLComponentValueList : public NotationBase {
public:
	MyValArray<MyXMLNamedValue*> NamedValues;
};

/*
XMLSequenceValue ::=
		XMLComponentValueList
		| empty
*/
class MyXMLSequenceValue : public NotationBase {
public:
	MyXMLComponentValueList* XMLComponentValueList;
};


#endif // _MY_SEQUENCE_TYPE_DEF_H_

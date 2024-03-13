#ifndef MY_CONSTRAINT_SPEC_DEF_H_
#define MY_CONSTRAINT_SPEC_DEF_H_


#include "MyNotationCommon.h"

class MyObjectSet;
class MyDefinedObjectClass;
class MyType;
class MyDefinedObjectSet;
class MyObject;
class MyValue;
class MyGovernor;

/*
UserDefinedConstraintParameter ::=
		Governor ":" Value
		| Governor ":" Object
		| DefinedObjectSet
		| Type
		| DefinedObjectClass
*/
class MyUserDefinedConstraintParameter : public NotationBase {
public:
	MyGovernor*           Governor;
	MyValue*              Value;
	MyObject*             Object;
	MyDefinedObjectSet*   DefinedObjectSet;
	MyType*               Type;
	MyDefinedObjectClass* DefinedObjectClass;
};

/*
UserDefinedConstraint ::=
		CONSTRAINED BY "{" UserDefinedConstraintParameter "," * "}"
*/
class MyUserDefinedConstraint : public NotationBase {
public:
	MyValArray<MyUserDefinedConstraintParameter*> UserDefinedConstraintParameters;
};


/*SimpleTableConstraint ::= ObjectSet*/
typedef MyObjectSet MySimpleTableConstraint;

/*Level ::= "." Level | empty*/
class MyLevel : public NotationBase {
	
};

/*
ComponentIdList ::= identifier "." +
*/
class MyComponentIdList : public NotationBase {
public:
	MyArray<MyStringA> Identifiers;
};

/*
AtNotation ::=
		"@" ComponentIdList |
		"@" "." Level ComponentIdList
*/
class MyAtNotation : public NotationBase {
public:
	MyLevel*           Level;
	MyComponentIdList* ComponentIdList;
};

/*
ComponentRelationConstraint ::=
		"{" DefinedObjectSet "}" "{" AtNotation "," + "}"
*/
class MyComponentRelationConstraint : public NotationBase {
public:
	MyDefinedObjectSet*       DefinedObjectSet;
	MyValArray<MyAtNotation*> AtNotations;
};

/*
TableConstraint ::=
		SimpleTableConstraint |
		ComponentRelationConstraint
*/
class MyTableConstraint : public NotationBase {
public:
	MySimpleTableConstraint*       SimpleTableConstraint;
	MyComponentRelationConstraint* ComponentRelationConstraint;
};


/*
ContentsConstraint ::=
		CONTAINING Type
		| ENCODED BY Value
		| CONTAINING Type ENCODED BY Value
*/
class MyContentsConstraint : public NotationBase {
public:
	MyType*  ContainingType;
	MyValue* EncodedByValue;
};

/*
GeneralConstraint ::=
		UserDefinedConstraint
		| TableConstraint
		| ContentsConstraint
*/
class MyGeneralConstraint : public NotationBase {
public:
	MyUserDefinedConstraint* UserDefinedConstraint;
	MyTableConstraint*       TableConstraint;
	MyContentsConstraint*    ContentsConstraint;
};








#endif // MY_CONSTRAINT_SPEC_DEF_H_

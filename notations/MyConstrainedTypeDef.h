#ifndef _MY_CONSTRAINED_TYPE_DEF_H_
#define _MY_CONSTRAINED_TYPE_DEF_H_


#include "MyNotationCommon.h"


class MyGeneralConstraint;
class MyNamedType;
class MySizeConstraint;
class MyType;
class MyExceptionSpec;
class MyElementSetSpecs;

/*
SubtypeConstraint ::= ElementSetSpecs
*/
typedef MyElementSetSpecs MySubtypeConstraint;

/*
ConstraintSpec ::=
		SubtypeConstraint
		| GeneralConstraint
*/
class MyConstraintSpec : public NotationBase {
public:
	MySubtypeConstraint* SubtypeConstraint;
	MyGeneralConstraint* GeneralConstraint;
};

/*
Constraint ::= "(" ConstraintSpec ExceptionSpec ")"
*/
class MyConstraint : public NotationBase {
public:
	MyConstraintSpec* ConstraintSpec;
	MyExceptionSpec*  ExceptionSpec;
};


/*
TypeWithConstraint ::=
		SET Constraint OF Type
		| SET SizeConstraint OF Type
		| SEQUENCE Constraint OF Type
		| SEQUENCE SizeConstraint OF Type
		| SET Constraint OF NamedType
		| SET SizeConstraint OF NamedType
		| SEQUENCE Constraint OF NamedType
		| SEQUENCE SizeConstraint OF NamedType
*/
class MyTypeWithConstraint : public NotationBase {
public:
	bool              Set; // Or Sequence
	MyConstraint*     Constraint;
	MySizeConstraint* SizeConstraint;
	MyType*           Type;
	MyNamedType*      NamedType;
};

/*
ConstrainedType ::=
		Type Constraint
		| TypeWithConstraint
*/
class MyConstrainedType : public NotationBase {
public:
	MyType*               Type;
	MyConstraint*         Constraint;
	MyTypeWithConstraint* TypeWithConstraint;
};





#endif // _MY_CONSTRAINED_TYPE_DEF_H_


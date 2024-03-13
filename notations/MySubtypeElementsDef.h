#ifndef _MY_SUBTYPE_ELEMENTS_DEF_H_
#define _MY_SUBTYPE_ELEMENTS_DEF_H_



#include "MyNotationCommon.h"

class MyConstraint;
class MyType;
class MyValue;

/*SingleValue ::= Value*/
typedef MyValue MySingleValue;

/*Includes ::= INCLUDES | empty*/
class MyIncludes : public NotationBase {
public:
	MyStringA Value;
};

/*
ContainedSubtype ::= Includes Type
*/
class MyContainedSubtype : public NotationBase {
public:
	MyIncludes* Includes;
	MyType*     Type;
};

/*
LowerEndValue ::= Value | MIN
*/
class MyLowerEndValue : public NotationBase {
public:
	MyValue* Value;
	bool     MIN;
};

/*
UpperEndValue ::= Value | MAX
*/
class MyUpperEndValue : public NotationBase {
public:
	MyValue* Value;
	bool     MAX;
};

/*
LowerEndpoint ::= LowerEndValue | LowerEndValue "<"
*/
class MyLowerEndpoint : public NotationBase {
public:
	MyLowerEndValue* LowerEndValue;
	bool             LessThan;
};

/*
UpperEndpoint ::= UpperEndValue | "<" UpperEndValue
*/
class MyUpperEndpoint : public NotationBase {
public:
	MyUpperEndValue* UpperEndValue;
	bool             LessThan;
};

/*ValueRange ::= LowerEndpoint ".." UpperEndpoint*/
class MyValueRange : public NotationBase {
public:
	MyLowerEndpoint* LowerEndpoint;
	MyUpperEndpoint* UpperEndpoint;
};

/*
SizeConstraint ::= SIZE Constraint
*/
class MySizeConstraint : public NotationBase {
public:
	MyConstraint* Constraint;
};

/*
TypeConstraint ::= Type
*/
typedef MyType MyTypeConstraint;

/*
PermittedAlphabet ::= FROM Constraint
*/
class MyPermittedAlphabet : public NotationBase {
public:
	MyConstraint* Constraint;
};

/*
SingleTypeConstraint ::= Constraint
*/
typedef MyConstraint MySingleTypeConstraint;

/*
ValueConstraint ::= Constraint | empty
*/
class MyValueConstraint : public NotationBase {
public:
	MyConstraint* Constraint;
	bool          Empty;
};

/*
PresenceConstraint ::= PRESENT | ABSENT | OPTIONAL | empty
*/
class MyPresenceConstraint : public NotationBase {
public:
	MyStringA Value;
};

/*
PatternConstraint ::= PATTERN Value
*/
class MyPatternConstraint : public NotationBase {
public:
	MyValue* Value;
};

/*
ComponentConstraint ::= ValueConstraint PresenceConstraint
*/
class MyComponentConstraint : public NotationBase {
public:
	MyValueConstraint*    ValueConstraint;
	MyPresenceConstraint* PresenceConstraint;
};

/*
NamedConstraint ::=
	identifier ComponentConstraint
*/
class MyNamedConstraint : public NotationBase {
public:
	MyStringA              Identifier;
	MyComponentConstraint* ComponentConstraint;
};

/*
TypeConstraints ::=
		NamedConstraint
		| NamedConstraint "," TypeConstraints
*/
class MyTypeConstraints : public NotationBase {
public:
	MyNamedConstraint* NamedConstraint;
	MyTypeConstraints* TypeConstraints;
};

/*
FullSpecification ::= "{" TypeConstraints "}"
*/
class MyFullSpecification : public NotationBase {
public:
	MyTypeConstraints* TypeConstraints;
};

/*
PartialSpecification ::= "{" "..." "," TypeConstraints "}"
*/
class MyPartialSpecification : public NotationBase {
public:
	MyTypeConstraints* TypeConstraints;
};


/*
MultipleTypeConstraints ::=
		FullSpecification
		| PartialSpecification
*/
class MyMultipleTypeConstraints : public NotationBase {
public:
	MyFullSpecification*    FullSpecification;
	MyPartialSpecification* PartialSpecification;
};

/*
InnerTypeConstraints ::=
		WITH COMPONENT SingleTypeConstraint
		| WITH COMPONENTS MultipleTypeConstraints
*/
class MyInnerTypeConstraints : public NotationBase {
public:
	MySingleTypeConstraint*    SingleTypeConstraint;
	MyMultipleTypeConstraints* MultipleTypeConstraints;
};


/*
SubtypeElements ::=
		SingleValue
		| ContainedSubtype
		| ValueRange
		| PermittedAlphabet
		| SizeConstraint
		| TypeConstraint
		| InnerTypeConstraints
		| PatternConstraint
*/
class MySubtypeElements : public NotationBase {
public:
	MySingleValue*          SingleValue;
	MyContainedSubtype*     ContainedSubtype;
	MyValueRange*           ValueRange;
	MyPermittedAlphabet*    PermittedAlphabet;
	MySizeConstraint*       SizeConstraint;
	MyTypeConstraint*       TypeConstraint;
	MyInnerTypeConstraints* InnerTypeConstraints;
	MyPatternConstraint*    PatternConstraint;
};









#endif // _MY_SUBTYPE_ELEMENTS_DEF_H_

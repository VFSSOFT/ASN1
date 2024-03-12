#ifndef _MY_ELEMENT_SET_SPEC_DEF_H_
#define _MY_ELEMENT_SET_SPEC_DEF_H_


#include "MyNotationCommon.h"

class MyExclusions;
class MyElementSetSpec;
class MyObjectSetElements;
class MySubtypeElements;


/*
IntersectionMark ::= "^" | INTERSECTION
*/
class MyIntersectionMark : public NotationBase {
public:
	MyStringA Value;
};

/*
UnionMark ::= "|" | UNION
*/
class MyUnionMark : public NotationBase {
public:
	MyStringA Value;
};

/*
Elements ::=
		SubtypeElements
		| ObjectSetElements
		| "(" ElementSetSpec ")"
*/
class MyElements : public NotationBase {
public:
	MySubtypeElements*   SubtypeElements;
	MyObjectSetElements* ObjectSetElements;
	MyElementSetSpec*    ElementSetSpec;
};

/*Elems ::= Elements*/
typedef MyElements MyElems;

/*
IntersectionElements ::= Elements | Elems Exclusions
*/
class MyIntersectionElements : public NotationBase {
public:
	MyElements*   Elements;
	MyElems*      Elems;
	MyExclusions* Exclusions;
};

/*
Intersections ::= IntersectionElements
		| IElems IntersectionMark IntersectionElements
*/
class MyIntersections : public NotationBase {
public:
	MyIElems*               IElems;
	MyIntersectionMark*     IntersectionMark;
	MyIntersectionElements* IntersectionElements; // used twice here
};

typedef MyIntersections MyIElems;

/*
Unions ::= Intersections
		| UElems UnionMark Intersections
*/
class MyUnions : public NotationBase {
public:
	MyUElems*        UElems;
	MyUnionMark*     UnionMark;
	MyIntersections* Intersections;// used twice here
};

/*UElems ::= Unions*/
typedef MyUnions MyUElems;

/*
Exclusions ::= EXCEPT Elements
*/
class MyExclusions : public NotationBase {
public:
	MyElements* Elements;
};

/*
ElementSetSpec ::= Unions
		| ALL Exclusions
*/
class MyElementSetSpec : public NotationBase {
public:
	MyUnions*     Unions;
	MyExclusions* AllExclusions;
};

/*RootElementSetSpec ::= ElementSetSpec*/
typedef MyElementSetSpec MyRootElementSetSpec;

/*AdditionalElementSetSpec ::= ElementSetSpec*/
typedef MyElementSetSpec MyAdditionalElementSetSpec;

/*
ElementSetSpecs ::=
		RootElementSetSpec
		| RootElementSetSpec "," "..."
		| RootElementSetSpec "," "..." "," AdditionalElementSetSpec
*/
class MyElementSetSpecs : public NotationBase {
public:
	MyRootElementSetSpec*       RootElementSetSpec;
	MyAdditionalElementSetSpec* AdditionalElementSetSpec;
};



#endif // _MY_ELEMENT_SET_SPEC_DEF_H_


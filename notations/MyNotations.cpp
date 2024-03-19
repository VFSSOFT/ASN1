#ifndef _MY_NOTATIONS_CPP_
#define _MY_NOTATIONS_CPP_


#include "MyTypeValueDef.h"
#include "MyConstrainedTypeDef.h"


MyType::MyType(): BuiltinType(NULL), ReferencedType(NULL), ConstrainedType(NULL) {

}
MyType::~MyType() {
	delete BuiltinType;
	delete ReferencedType;
	delete ConstrainedType;
}

MyBuiltinType::MyBuiltinType():
  BitStringType(NULL),
	BooleanType(NULL),
	ChoiceType(NULL),
	EnumeratedType(NULL),
	ExternalType(NULL),
	IntegerType(NULL),
	NullType(NULL),
	ObjectClassFieldType(NULL),
	ObjectIDType(NULL),
	OctetStringType(NULL),
	RealType(NULL),
	SequenceType(NULL),
	SequenceOfType(NULL),
	SetType(NULL),
	SetOfType(NULL),
	TaggedType(NULL)
{

}
MyBuiltinType::~MyBuiltinType() {
	delete BitStringType; 
	delete BooleanType;
	delete ChoiceType;
	delete EnumeratedType;
	delete ExternalType;
	delete IntegerType;
	delete NullType;
	delete ObjectClassFieldType;
	delete ObjectIDType;
	delete OctetStringType;
	delete RealType;
	delete SequenceType;
	delete SequenceOfType;
	delete SetType;
	delete SetOfType;
	delete TaggedType;
}

MyConstrainedType::MyConstrainedType():
	Type(NULL),
	Constraint(NULL),
	TypeWithConstraint(NULL)
{

}
MyConstrainedType::~MyConstrainedType() {
	delete Type;
	delete Constraint;
	delete TypeWithConstraint;
}

#endif // _MY_NOTATIONS_CPP_
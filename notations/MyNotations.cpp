#ifndef _MY_NOTATIONS_CPP_
#define _MY_NOTATIONS_CPP_


#include "MyTypeValueDef.h"
#include "MyConstrainedTypeDef.h"
#include "MyModule.h"
#include "MyCharacterStringType.h"


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


MyAssignment::MyAssignment() : 
	TypeAssignment(NULL),
	ValueAssignment(NULL),
	ValueSetTypeAssignment(NULL),
	ObjectClassAssignment(NULL),
	ObjectAssignment(NULL),
	ObjectSetAssignment(NULL),
	ParameterizedAssignment(NULL)
{

}
MyAssignment::~MyAssignment() {
	delete TypeAssignment;
	delete ValueAssignment;
	delete ValueSetTypeAssignment;
	delete ObjectClassAssignment;
	delete ObjectAssignment;
	delete ObjectSetAssignment;
	delete ParameterizedAssignment;
}

MyCharStringType::MyCharStringType() :
	RestrictedCharStringType(NULL),
	UnrestrictedCharStringType(NULL)
{
}
MyCharStringType::~MyCharStringType() {
	delete RestrictedCharStringType;
	delete UnrestrictedCharStringType;
}

#endif // _MY_NOTATIONS_CPP_
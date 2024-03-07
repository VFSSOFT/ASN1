#ifndef _MY_TYPE_VALUE_DEF_H_
#define _MY_TYPE_VALUE_DEF_H_

#include "MyNotationCommon.h"
#include "MyRefTypeValueDef.h"


/*
BuiltinType ::=
		BitStringType
		| BooleanType
		| CharacterStringType
		| ChoiceType
		| EmbeddedPDVType
		| EnumeratedType
		| ExternalType
		| InstanceOfType
		| IntegerType
		| NullType
		| ObjectClassFieldType
		| ObjectIdentifierType
		| OctetStringType
		| RealType
		| RelativeOIDType
		| SequenceType
		| SequenceOfType
		| SetType
		| SetOfType
		| TaggedType
*/

class MyBuiltinType : public NotationBase {
public:

};

/*
ReferencedType ::=
		DefinedType
		| UsefulType
		| SelectionType
		| TypeFromObject
		| ValueSetFromObjects
*/
class MyReferencedType : public NotationBase {
public:
	MyDefinedType* DefinedType;
};

/*
	Type ::= BuiltinType | ReferencedType | ConstrainedType
*/
class MyType : public NotationBase {
public:
	MyBuiltinType* BuiltinType;
};

/*
NamedType ::= identifier Type
*/
class MyNamedType : public NotationBase {
public:
	MyStringA  Identifier;
	MyType*    Type;
};

/*
BuiltinValue ::=
		BitStringValue
		| BooleanValue
		| CharacterStringValue
		| ChoiceValue
		| EmbeddedPDVValue
		| EnumeratedValue
		| ExternalValue
		| InstanceOfValue
		| IntegerValue
		| NullValue
		| ObjectIdentifierValue
		| OctetStringValue
		| RealValue
		| RelativeOIDValue
		| SequenceValue
		| SequenceOfValue
		| SetValue
		| SetOfValue
		| TaggedValue
*/
class MyBuiltinValue : public NotationBase {

};

/*
XMLBuiltinValue ::=
		XMLBitStringValue
		| XMLBooleanValue
		| XMLCharacterStringValue
		| XMLChoiceValue
		| XMLEmbeddedPDVValue
		| XMLEnumeratedValue
		| XMLExternalValue
		| XMLInstanceOfValue
		| XMLIntegerValue
		| XMLNullValue
		| XMLObjectIdentifierValue
		| XMLOctetStringValue
		| XMLRealValue
		| XMLRelativeOIDValue
		| XMLSequenceValue
		| XMLSequenceOfValue
		| XMLSetValue
		| XMLSetOfValue
		| XMLTaggedValue
*/
class MyXMLBuiltinValue : public NotationBase {

};

/*
ReferencedValue ::=
		DefinedValue
		| ValueFromObject
*/
class MyReferencedValue : public NotationBase {
public:
	MyDefinedValue* DefinedValue;
};

/*
XMLValue ::=
		XMLBuiltinValue
		| XMLObjectClassFieldValue
*/
class MyXMLValue : public NotationBase {
public:

};

/*
Value ::=
		BuiltinValue
		| ReferencedValue
		| ObjectClassFieldValue
*/
class MyValue : public NotationBase {
public:
	MyBuiltinValue* BuiltinValue;
};

/*
NamedValue ::= identifier Value
*/
class MyNamedValue : public NotationBase {
public:
	MyStringA  Identifier;
	MyValue*   Value;
};

/*
XMLNamedValue ::= "<" & identifier ">" XMLValue "</" & identifier ">"
*/
class MyXMLNamedValue : public NotationBase {
public:

};

#endif // _MY_TYPE_VALUE_DEF_H_

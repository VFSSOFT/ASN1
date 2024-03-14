#ifndef _MY_INFORMATION_OBJECT_DEF_H_
#define _MY_INFORMATION_OBJECT_DEF_H_

#include "MyNotationCommon.h"

class MyTokenOrGroupSpec;
class MyObjectFromObject;
class MyObjectSetFromObjects;
class MyParameterizedObject;
class MyPrimitiveFieldName;
class MyType;
class MyValue;
class MyValueSet;
class MyObject;
class MyBuiltinValue;
class MyReferencedValue;

/*
ExternalObjectClassReference ::=
		modulereference
		"."
		objectclassreference
*/
class MyExternalObjectClassReference : public NotationBase {
public:
	MyStringA ModuleReference;
	MyStringA ObjectClassReference;
};


/*
ExternalObjectReference ::=
		modulereference
		"."
		objectreference
*/
class MyExternalObjectReference : public NotationBase {
public:
	MyStringA ModuleReference;
	MyStringA ObjectReference;
};

/*
ExternalObjectSetReference ::=
		modulereference
		"."
		objectsetreference
*/
class MyExternalObjectSetReference : public NotationBase {
public:
	MyStringA ModuleReference;
	MyStringA ObjectSetReference;
};

/*
UsefulObjectClassReference ::= TYPE-IDENTIFIER | ABSTRACT-SYNTAX
*/
class MyUsefulObjectClassReference : public NotationBase {
public:
	MyStringA Value;
};

/*
Literal ::=
		word
		| ","
*/
class MyLiteral : public NotationBase {
public:
	MyStringA Value;
};

/*
RequiredToken ::=
		Literal
		| PrimitiveFieldName
*/
class MyRequiredToken : public NotationBase {
public:
	MyLiteral*            Literal;
	MyPrimitiveFieldName* PrimitiveFieldName;
};


/*
OptionalGroup ::= "[" TokenOrGroupSpec empty + "]"
*/
class MyOptionalGroup : public NotationBase {
public:
	MyValArray<MyTokenOrGroupSpec*> TokenOrGroupSpecs;
};

/*
TokenOrGroupSpec ::= RequiredToken | OptionalGroup
*/
class MyTokenOrGroupSpec : public NotationBase {
public:
	MyRequiredToken* RequiredToken;
	MyOptionalGroup* OptionalGroup;
};

/*
SyntaxList ::= "{" TokenOrGroupSpec empty + "}
*/
class MySyntaxList : public NotationBase {
public:
	MyValArray<MyTokenOrGroupSpec*> TokenOrGroupSpecs;
};


/*
DefinedObjectClass ::=
		ExternalObjectClassReference
		| objectclassreference
		| UsefulObjectClassReference
*/
class MyDefinedObjectClass : public NotationBase {
public:
	MyExternalObjectClassReference* ExternalObjectClassReference;
	MyStringA                      ObjectClassReference;
	MyUsefulObjectClassReference*   UsefulObjectClassReference;
};

/*
DefinedObject ::=
		ExternalObjectReference
		| objectreference
*/
class MyDefinedObject : public NotationBase {
public:
	MyExternalObjectReference* ExternalObjectReference;
	MyStringA                  ObjectReference;
};

/*
DefinedObjectSet ::=
		ExternalObjectSetReference
		| objectsetreference
*/
class MyDefinedObjectSet : public NotationBase {
public:
	MyExternalObjectSetReference* ExternalObjectSetReference;
	MyStringA                     ObjectSetReference;
};


/*
TypeOptionalitySpec ::= OPTIONAL | DEFAULT Type
*/
class MyTypeOptionalitySpec : public NotationBase {
public:
	bool    Optional;
	bool    Default;
	MyType* Type;
};

/*
TypeFieldSpec ::=
		typefieldreference
		TypeOptionalitySpec?
*/
class MyTypeFieldSpec : public NotationBase {
public:
	MyStringA              TypeFieldReference;
	MyTypeOptionalitySpec* TypeOptionalitySpec;
};

/*
ValueOptionalitySpec ::= OPTIONAL | DEFAULT Value
*/
class MyValueOptionalitySpec : public NotationBase {
public:
	bool     Optional;
	bool     Default;
	MyValue* Value;
};

/*
FixedTypeValueFieldSpec ::=
		valuefieldreference
		Type
		UNIQUE ?
		ValueOptionalitySpec ?
*/
class MyFixedTypeValueFieldSpec : public NotationBase {
public:
	MyStringA               ValueFieldReference;
	MyType*                 Type;
	bool                    Unique;
	MyValueOptionalitySpec* ValueOptionalitySpec;
};

/*
PrimitiveFieldName ::=
		typefieldreference
		| valuefieldreference
		| valuesetfieldreference

A "typefieldreference" shall consist of an ampersand ("&") immediately followed by a sequence of characters as specified
for a "typereference" in Rec. ITU-T X.680 | ISO/IEC 8824-1, 12.2.

A "valuefieldreference" shall consist of an ampersand ("&") immediately followed by a sequence of characters as specified
for a "valuereference" in Rec. ITU-T X.680 | ISO/IEC 8824-1, 12.4.

A "valuesetfieldreference" shall consist of an ampersand ("&") immediately followed by a sequence of characters as
specified for a "typereference" in Rec. ITU-T X.680 | ISO/IEC 8824-1, 12.2.

*/
class MyPrimitiveFieldName : public NotationBase {
public:
	MyStringA TypeFieldReference;
	MyStringA ValueFieldReference;
	MyStringA ValueSetFieldReference;
};

/*
FieldName ::= PrimitiveFieldName "." +
*/
class MyFieldName : public NotationBase {
public:
	MyPrimitiveFieldName* PrimitiveFieldName;
};

/*
VariableTypeValueFieldSpec ::=
		valuefieldreference
		FieldName
		ValueOptionalitySpec?
*/
class MyVariableTypeValueFieldSpec : public NotationBase {
public:
	MyStringA               ValueFieldReference;
	MyFieldName*            FieldName;
	MyValueOptionalitySpec* ValueOptionalitySpec;
};


/*
ValueSetOptionalitySpec ::= OPTIONAL | DEFAULT ValueSet
*/
class MyValueSetOptionalitySpec : public NotationBase {
public:
	bool        Optional;
	bool        Default;
	MyValueSet* ValueSet;
};

/*
FixedTypeValueSetFieldSpec ::=
		valuesetfieldreference
		Type
		ValueSetOptionalitySpec?
*/
class MyFixedTypeValueSetFieldSpec : public NotationBase {
public:
	MyStringA                  ValueSetFieldReference;
	MyType*                    Type;
	MyValueSetOptionalitySpec* ValueSetOptionalitySpec;
};

/*
VariableTypeValueSetFieldSpec ::=
		valuesetfieldreference
		FieldName
		ValueSetOptionalitySpec?
*/
class MyVariableTypeValueSetFieldSpec : public NotationBase {
public:
	MyStringA                  ValueSetFieldReference;
	MyFieldName*               FieldName;
	MyValueSetOptionalitySpec* ValueSetOptionalitySpec;
};

/*
ObjectOptionalitySpec ::= OPTIONAL | DEFAULT Object
*/
class MyObjectOptionalitySpec : public NotationBase {
public:
	bool      Optional;
	bool      Default;
	MyObject* Object;
};


/*
ObjectFieldSpec ::=
		objectfieldreference
		DefinedObjectClass
		ObjectOptionalitySpec ?
*/
class MyObjectFieldSpec : public NotationBase {
public:
	MyStringA                ObjectFieldReference;
	MyDefinedObjectClass*    DefinedObjectClass;
	MyObjectOptionalitySpec* ObjectOptionalitySpec;
};

/*
ObjectSetOptionalitySpec ::= OPTIONAL | DEFAULT ObjectSet
*/
class MyObjectSetOptionalitySpec : public NotationBase {
public:
	bool         Optional;
	bool         Default;
	MyObjectSet* ObjectSet;
};

/*
ObjectSetFieldSpec ::=
		objectsetfieldreference
		DefinedObjectClass
		ObjectSetOptionalitySpec ?
*/
class MyObjectSetFieldSpec : public NotationBase {
public:
	MyStringA                   ObjectSetFieldReference;
	MyDefinedObjectClass*       DefinedObjectClass;
	MyObjectSetOptionalitySpec* ObjectSetOptionalitySpec;
};


/*
FieldSpec ::=
		TypeFieldSpec
		| FixedTypeValueFieldSpec
		| VariableTypeValueFieldSpec
		| FixedTypeValueSetFieldSpec
		| VariableTypeValueSetFieldSpec
		| ObjectFieldSpec
		| ObjectSetFieldSpec
*/
class MyFieldSpec : public NotationBase {
public:
	MyTypeFieldSpec*                 TypeFieldSpec;
	MyFixedTypeValueFieldSpec*       FixedTypeValueFieldSpec;
	MyVariableTypeValueFieldSpec*    VariableTypeValueFieldSpec;
	MyFixedTypeValueSetFieldSpec*    FixedTypeValueSetFieldSpec;
	MyVariableTypeValueSetFieldSpec* VariableTypeValueSetFieldSpec;
	MyObjectFieldSpec*               ObjectFieldSpec;
	MyObjectSetFieldSpec*            ObjectSetFieldSpec;
};

/*
WithSyntaxSpec ::= WITH SYNTAX SyntaxList
*/
class MyWithSyntaxSpec : public NotationBase {
public:
	MySyntaxList* SyntaxList;
};

/*
ObjectClassDefn ::=
		CLASS
		"{" FieldSpec "," + "}"
		WithSyntaxSpec?
*/
class MyObjectClassDefn : public NotationBase {
public:
	MyValArray<MyFieldSpec*> FieldSpecs;
	MyWithSyntaxSpec*        WithSyntaxSpec;
};

/*
ObjectClass ::=
		DefinedObjectClass
		| ObjectClassDefn
		| ParameterizedObjectClass
*/
class MyObjectClass : public NotationBase {
public:
	MyDefinedObjectClass*       DefinedObjectClass;
	MyObjectClassDefn*          ObjectClassDefn;
	MyParameterizedObjectClass* ParameterizedObjectClass;
};

/*
ObjectClassAssignment ::=
		objectclassreference
		"::="
		ObjectClass
*/
class MyObjectClassAssignment : public NotationBase {
public:
	MyStringA      ObjectClassReference;
	MyObjectClass* ObjectClass;
};


/*
Setting ::=
		Type
		| Value
		| ValueSet
		| Object
		| ObjectSet
*/
class MySetting : public NotationBase {
public:
	MyType*      Type;
	MyValue*     Value;
	MyValueSet*  ValueSet;
	MyObject*    Object;
	MyObjectSet* ObjectSet;
};


/*
FieldSetting ::= PrimitiveFieldName Setting
*/
class MyFieldSetting : public NotationBase {
public:
	MyPrimitiveFieldName* PrimitiveFieldName;
	MySetting*            Setting;
};

/*
DefaultSyntax ::= "{" FieldSetting "," * "}"
*/
class MyDefaultSyntax : public NotationBase {
public:
	MyValArray<MyFieldSetting*> FieldSettings;
};


/*
DefinedSyntaxToken ::=
		Literal
		| Setting
*/
class MyDefinedSyntaxToken : public NotationBase {
public:
	MyLiteral* Literal;
	MySetting* Setting;
};

/*
DefinedSyntax ::= "{" DefinedSyntaxToken empty * "}"
*/
class MyDefinedSyntax : public NotationBase {
public:
	MyValArray<MyDefinedSyntaxToken*> DefinedSyntaxTokens;
};

/*
ObjectDefn ::=
		DefaultSyntax
		| DefinedSyntax
*/
class MyObjectDefn : public NotationBase {
public:
	MyDefaultSyntax* DefaultSyntax;
	MyDefinedSyntax* DefinedSyntax;
};

/*
Object ::=
		DefinedObject
		| ObjectDefn
		| ObjectFromObject
		| ParameterizedObj
*/
class MyObject : public NotationBase {
public:
	MyDefinedObject*       DefinedObject;
	MyObjectDefn*          ObjectDefn;
	MyObjectFromObject*    ObjectFromObject;
	MyParameterizedObject* ParameterizedObject;
};

/*
ObjectAssignment ::=
		objectreference
		DefinedObjectClass
		"::="
		Object
*/
class MyObjectAssignment : public NotationBase {
public:
	MyStringA             ObjectReference;
	MyDefinedObjectClass* DefinedObjectClass;
	MyObject*             Object;
};


/*
ObjectSetElements ::=
		Object
		| DefinedObjectSet
		| ObjectSetFromObjects
		| ParameterizedObjectSet
*/
class MyObjectSetElements : public NotationBase {
public:
	MyObject*                 Object;
	MyDefinedObjectSet*       DefinedObjectSet;
	MyObjectSetFromObjects*   ObjectSetFromObjects;
	MyParameterizedObjectSet* ParameterizedObjectSet;
};

/*
ObjectSetSpec ::=
		RootElementSetSpec
		| RootElementSetSpec "," "..."
		| "..."
		| "..." "," AdditionalElementSetSpec
		| RootElementSetSpec "," "..." "," AdditionalElementSetSpec
*/
class MyObjectSetSpec : public NotationBase {
public:
	
};

/*
ObjectSet ::= "{" ObjectSetSpec "}"
*/
class MyObjectSet : public NotationBase {
public:
	MyObjectSetSpec* ObjectSetSpec;
};

/*
ObjectSetAssignment ::=
		objectsetreference
		DefinedObjectClass
		"::="
		ObjectSet
*/
class MyObjectSetAssignment : public NotationBase {
public:
	MyStringA             ObjectSetReference;
	MyDefinedObjectClass* DefinedObjectClass;
	MyObjectSet*          ObjectSet;
};


/*
ObjectClassFieldType ::=
		DefinedObjectClass
		"."
		FieldName
*/
class MyObjectClassFieldType : public NotationBase {
public:
	MyDefinedObjectClass* DefinedObjectClass;
	MyFieldName*          FieldName;
};


/*
OpenTypeFieldVal ::= Type ":" Value
*/
class MyOpenTypeFieldVal : public NotationBase {
public:
	MyType*   Type;
	MyValue*  Value;
};

/*
FixedTypeFieldVal ::= BuiltinValue | ReferencedValue
*/
class MyFixedTypeFieldVal : public NotationBase {
public:
	MyBuiltinValue*    BuiltinValue;
	MyReferencedValue* ReferencedValue;
};


/*
ObjectClassFieldValue ::=
		OpenTypeFieldVal
		| FixedTypeFieldVal
*/
class MyObjectClassFieldValue : public NotationBase {
public:
	MyOpenTypeFieldVal*  OpenTypeFieldVal;
	MyFixedTypeFieldVal* FixedTypeFieldVal;
};

/*
ReferencedObjects ::=
		DefinedObject
		| ParameterizedObject
		| DefinedObjectSet
		| ParameterizedObjectSet
*/
class MyReferencedObjects : public NotationBase {
public:
	MyDefinedObject*          DefinedObject;
	MyParameterizedObject*    ParameterizedObject;
	MyDefinedObjectSet*       DefinedObjectSet;
	MyParameterizedObjectSet* ParameterizedObjectSet;
};

/*
ObjectSetFromObjects ::=
		ReferencedObjects
		"."
		FieldName
*/
class MyObjectSetFromObjects : public NotationBase {
public:
	MyReferencedObjects* ReferencedObjects;
	MyFieldName*         FieldName;
};

/*
ObjectFromObject ::=
		ReferencedObjects
		"."
		FieldName
*/
class MyObjectFromObject : public NotationBase {
public:
	MyReferencedObjects* ReferencedObjects;
	MyFieldName*         FieldName;
};


/*
TypeFromObject ::=
		ReferencedObjects
		"."
		FieldName
*/
class MyTypeFromObject : public NotationBase {
public:
	MyReferencedObjects* ReferencedObjects;
	MyFieldName*         FieldName;
};

/*
ValueSetFromObjects ::=
		ReferencedObjects
		"."
		FieldName
*/
class MyValueSetFromObjects : public NotationBase {
public:
	MyReferencedObjects* ReferencedObjects;
	MyFieldName*         FieldName;
};

/*
ValueFromObject ::=
		ReferencedObjects
		"."
		FieldName
*/
class MyValueFromObject : public NotationBase {
public:
	MyReferencedObjects* ReferencedObjects;
	MyFieldName*         FieldName;
};

/*
InformationFromObjects ::=
		ValueFromObject
		| ValueSetFromObjects
		| TypeFromObject
		| ObjectFromObject
		| ObjectSetFromObjects
*/
class MyInformationFromObjects : public NotationBase {
public:
	MyValueFromObject*      ValueFromObject;
	MyValueSetFromObjects*  ValueSetFromObjects;
	MyTypeFromObject*       TypeFromObject;
	MyObjectFromObject*     ObjectFromObject;
	MyObjectSetFromObjects* ObjectSetFromObjects;
};

#endif // _MY_INFORMATION_OBJECT_DEF_H_


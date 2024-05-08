#ifndef _MY_PARSER_H_
#define _MY_PARSER_H_

#include "MyTokenizer.h"
#include "../notations/MyModule.h"
#include "../notations/MyTypeValueDef.h"
#include "../notations/MyRelativeObjectIDType.h"
#include "../notations/MyEnumeratedTypeDef.h"
#include "../notations/MyBitstringTypeDef.h"
#include "../notations/MyBoolTypeDef.h"
#include "../notations/MyCharacterStringType.h"
#include "../notations/MyChoiceTypeDef.h"
#include "../notations/MyExternalTypeDef.h"
#include "../notations/MyNullTypeDef.h"
#include "../notations/MyOctetstringTypeDef.h"
#include "../notations/MyRealTypeDef.h"
#include "../notations/MySequenceTypeDef.h"
#include "../notations/MySequenceOfTypeDef.h"
#include "../notations/MySetTypeDef.h"
#include "../notations/MySetOfTypeDef.h"
#include "../notations/MyTaggedTypeDef.h"
#include "../notations/MySelectionTypesDef.h"
#include "../notations/MyConstrainedTypeDef.h"
#include "../notations/MyConstrainSpecDef.h"
#include "../notations/MyElementSetSpecificationDef.h"
#include "../notations/MySubtypeElementsDef.h"

class MyParser {
public:

	int Parse(const wchar_t* file);

private:
	MyModuleDef* ParseModuleDef(int& tokIdx);
	MyModuleID* ParseModuleIdentifier(int& tokIdx);
	MyTagDefault* ParseDefaultTag(int& tokIdx);
	MyExtensionDefault* ParseExtensionDefault(int& tokIdx);
	MyModuleBody* ParseModuleBody(int& tokIdx);
	MyExports* ParseExports(int& tokIdx);
	MyImports* ParseImports(int& tokIdx);
	MyAssignmentList* ParseAssignmentList(int& tokIdx);

	MyDefinitiveID* ParseDefinitiveIdentifier(int& tokIdx);
	MyDefinitiveObjIdComponentList* ParseDefinitiveObjIdComponentList(int& tokIdx);
	MyDefinitiveObjIdComponent* ParseDefinitiveObjIdComponent(int& tokIdx);
	MyNameForm* ParseNameForm(int& tokIdx);
	MyDefinitiveNumberForm* ParseDefinitiveNumberForm(int& tokIdx);
	MyDefinitiveNameAndNumberForm* ParseDefinitiveNameAndNumberForm(int& tokIdx);

	MySymbolsExported* ParseSymbolsExported(int& tokIdx);
	MySymbolList* ParseSymbolList(int& tokIdx);
	MySymbol* ParseSymbol(int& tokIdx);

	MySymbolsImported* ParseSymbolsImported(int& tokIdx);
	MySymbolsFromModuleList* ParseSymbolsFromModuleList(int& tokIdx);
	MySymbolsFromModule* ParseSymbolsFromModule(int& tokIdx);
	MyGlobalModuleReference* ParseGlobalModuleReference(int& tokIdx);
	MyAssignedID* ParseAssignedIdentifier(int& tokIdx);
	MyObjectIDValue* ParseObjectIDValue(int& tokIdx);
	MyObjIdComponentsList* ParseObjIdComponentsList(int& tokIdx);
	MyObjIdComponents* ParseObjIdComponents(int& tokIdx);
	MyNameAndNumberForm* ParseNameAndNumberForm(int& tokIdx);
	MyNumberForm* ParseNumberForm(int& tokIdx);

	MyAssignment* ParseAssignment(int& tokIdx);
	MyTypeAssignment* ParseTypeAssignment(int& tokIdx);
	MyValueAssignment* ParseValueAssignment(int& tokIdx);
	MyValueSetTypeAssignment* ParseValueSetTypeAssignment(int& tokIdx);
	MyObjectClassAssignment* ParseObjectClassAssignment(int& tokIdx);
	MyObjectAssignment* ParseObjectAssignment(int& tokIdx);
	MyObjectSetAssignment* ParseObjectSetAssignment(int& tokIdx);
	MyParameterizedAssignment* ParseParameterizedAssignment(int& tokIdx);
	MyParameterizedTypeAssignment* ParseParameterizedTypeAssignment(int& tokIdx);
	MyParameterizedValueAssignment* ParseParameterizedValueAssignment(int& tokIdx);
	MyParameterizedValueSetTypeAssignment* ParseParameterizedValueSetTypeAssignment(int& tokIdx);
	MyParameterizedObjectClassAssignment* ParseParameterizedObjectClassAssignment(int& tokIdx);
	MyParameterizedObjectAssignment* ParseParameterizedObjectAssignment(int& tokIdx);
	MyParameterizedObjectSetAssignment* ParseParameterizedObjectSetAssignment(int& tokIdx);

	MyType* ParseType(int& tokIdx);
	MyBuiltinType* ParseBuiltinType(int& tokIdx);
	MyBitStringType* ParseBitStringType(int& tokIdx);
	MyNamedBitList* ParseNamedBitList(int& tokIdx);
	MyNamedBit* ParseNamedBit(int& tokIdx);
	MyBooleanType* ParseBoolType(int& tokIdx);
	MyCharStringType* ParseCharStringType(int& tokIdx);
	MyChoiceType* ParseChoiceType(int& tokIdx);
	MyEnumeratedType* ParseEnumeratedType(int& tokIdx);
	MyExternalType* ParseExternalType(int& tokIdx);
	MyIntegerType* ParseIntegerType(int& tokIdx);
	MyNullType* ParseNullType(int& tokIdx);
	MyObjectClassFieldType* ParseObjectClassFieldType(int& tokIdx);
	MyObjectIDType* ParseObjectIDType(int& tokIdx);
	MyOctetStringType* ParseOctetStringType(int& tokIdx);
	MyRealType* ParseRealType(int& tokIdx);
	MySequenceType* ParseSequenceType(int& tokIdx);
	MySequenceOfType* ParseSequenceOfType(int& tokIdx);
	MySetType* ParseSetType(int& tokIdx);
	MySetOfType* ParseSetOfType(int& tokIdx);
	MyTaggedType* ParseTaggedType(int& tokIdx);
	MyReferencedType* ParseReferencedType(int& tokIdx);
	MyDefinedType* ParseDefinedType(int& tokIdx);
	MyExternalTypeReference* ParseExternalTypeReference(int& tokIdx);
	MyParameterizedType* ParseParameterizedType(int& tokIdx);
	MySimpleDefinedType* ParseSimpleDefinedType(int& tokIdx);
	MyActualParameterList* ParseActualParameterList(int& tokIdx);
	MyActualParameter* ParseActualParameter(int& tokIdx);
	MyParameterizedValueSetType* ParseParameterizedValueSetType(int& tokIdx);
	MySelectionType* ParseSelectionType(int& tokIdx);
	MyTypeFromObject* ParseTypeFromObject(int& tokIdx);
	MyReferencedObjects* ParseReferencedObjects(int& tokIdx);
	MyValueSetFromObjects* ParseValueSetFromObjects(int& tokIdx);
	MyRestrictedCharStringType* ParseRestrictedCharStringType(int& tokIdx);
	MyUnrestrictedCharStringType* ParseUnrestrictedCharStringType(int& tokIdx);

	MyConstrainedType* ParseConstrainedType(int& tokIdx, MyType* parsedType);
	MyConstraint* ParseConstraint(int& tokIdx);
	MyTypeWithConstraint* ParseTypeWithConstraint(int& tokIdx);
	MyIntersectionMark* ParseIntersectionMark(int& tokIdx);
	MyUnionMark* ParseUnionMark(int& tokIdx);
	MyIncludes* ParseIncludes(int& tokIdx);
	MyContainedSubtype* ParseContainedSubtype(int& tokIdx);
	MyLowerEndValue* ParseLowerEndValue(int& tokIdx);
	MyUpperEndValue* ParseUpperEndValue(int& tokIdx);
	MyLowerEndpoint* ParseLowerEndpoint(int& tokIdx);
	MyUpperEndpoint* ParseUpperEndpoint(int& tokIdx);
	MyValueRange* ParseValueRange(int& tokIdx);
	MySizeConstraint* ParseSizeConstraint(int& tokIdx);
	MyPermittedAlphabet* ParsePermittedAlphabet(int& tokIdx);
	MyValueConstraint* ParseValueConstraint(int& tokIdx);
	MyPresenceConstraint* ParsePresenceConstraint(int& tokIdx);
	MyPatternConstraint* ParsePatternConstraint(int& tokIdx);
	MyComponentConstraint* ParseComponentConstraint(int& tokIdx);
	MyNamedConstraint* ParseNamedConstraint(int& tokIdx);
	MyTypeConstraints* ParseTypeConstraints(int& tokIdx);
	MyFullSpecification* ParseFullSpecification(int& tokIdx);
	MyPartialSpecification* ParsePartialSpecification(int& tokIdx);
	MyMultipleTypeConstraints* ParseMultipleTypeConstraints(int& tokIdx);
	MyInnerTypeConstraints* ParseInnerTypeConstraints(int& tokIdx);
	MySubtypeElements* ParseSubtypeElements(int& tokIdx);
	MyUnions* ParseUnions(int& tokIdx);
	MyIntersections* ParseIntersections(int& tokIdx);
	MyIntersectionElements* ParseIntersectionElements(int& tokIdx);
	MyConstraintSpec* ParseConstraintSpec(int& tokIdx);

	MyGeneralConstraint* ParseGeneralConstraint(int& tokIdx);
	MyUserDefinedConstraint* ParseUserDefinedConstraint(int& tokIdx);
	MyUserDefinedConstraintParameter* ParseUserDefinedConstraintParameter(int& tokIdx);
	MyTableConstraint* ParseTableConstraint(int& tokIdx);
	MyComponentRelationConstraint* ParseComponentRelationConstraint(int& tokIdx);
	MyAtNotation* ParseAtNotation(int& tokIdx);
	MyComponentIdList* ParseComponentIdList(int& tokIdx);
	MyContentsConstraint* ParseContentsConstraint(int& tokIdx);

	MyElementSetSpecs* ParseElementSetSpecs(int& tokIdx);
	MyElementSetSpec* ParseElementSetSpec(int& tokIdx);
	MyExclusions* ParseExclusions(int& tokIdx);
	MyElements* ParseElements(int& tokIdx);
	MyObjectSetElements* ParseObjectSetElements(int& tokIdx);

	MyAlternativeTypeLists* ParseAlternativeTypeLists(int& tokIdx);
	MyAlternativeTypeList* ParseAlternativeTypeList(int& tokIdx);
	MyExtensionAndException* ParseExtensionAndException(int& tokIdx);
	MyExtensionAdditionAlternative* ParseExtensionAdditionAlternative(int& tokIdx);
	MyExtensionAdditionGroup* ParseExtensionAdditionGroup(int& tokIdx);
	MyExtensionAdditionList* ParseExtensionAdditionList(int& tokIdx);
	MyExtensionAddition* ParseExtensionAddition(int& tokIdx);
	MyExtensionAdditions* ParseExtensionAdditions(int& tokIdx);
	MyOptionalExtensionMarker* ParseOptionalExtensionMarker(int& tokIdx);
	MyExtensionEndMarker* ParseExtensionEndMarker(int& tokIdx);
	MyNamedType* ParseNamedType(int& tokIdx);
	MyExceptionSpec* ParseExceptionSpec(int& tokIdx);
	MyExceptionIdentification* ParseExceptionIdentification(int& tokIdx);
	MyExtensionAdditionAlternativeGroup* ParseExtensionAdditionAlternativeGroup(int& tokIdx);
	MyVersionNumber* ParseVersionNumber(int& tokIdx);
	MyEnumerations* ParseEnumerations(int& tokIdx);
	MyEnumeration* ParseEnumeration(int& tokIdx);
	MyEnumerationItem* ParseEnumerationItem(int& tokIdx);

	MySignedNumber* ParseSignedNumber(int& tokIdx);
	MyDefinedValue* ParseDefinedValue(int& tokIdx);
	MyValue* ParseValue(int& tokIdx);
	MyBuiltinValue* ParseBuiltinValue(int& tokIdx);
	MyBitStringValue* ParseBitStringValue(int& tokIdx);
	MyBooleanValue* ParseBooleanValue(int& tokIdx);
	MyChoiceValue* ParseChoiceValue(int& tokIdx);
	MyEnumeratedValue* ParseEnumeratedValue(int& tokIdx);
	MyExternalValue* ParseExternalValue(int& tokIdx);
	MyIntegerValue* ParseIntegerValue(int& tokIdx);
	MyNullValue* ParseNullValue(int& tokIdx);
	MyOctetStringValue* ParseOctetStringValue(int& tokIdx);
	MyRealValue* ParseRealValue(int& tokIdx);
	MySequenceValue* ParseSequenceValue(int& tokIdx);
	MySequenceOfValue* ParseSequenceOfValue(int& tokIdx);
	MySetValue* ParseSetValue(int& tokIdx);
	MySetOfValue* ParseSetOfValue(int& tokIdx);
	MyTaggedValue* ParseTaggedValue(int& tokIdx);
	MyValueSet* ParseValueSet(int& tokIdx);
	MyObject* ParseObject(int& tokIdx);
	MyObjectDefn* ParseObjectDefn(int& tokIdx);
	MyObjectSetSpec* ParseObjectSetSpec(int& tokIdx);
	MyObjectSet* ParseObjectSet(int& tokIdx);
	MyObjectClass* ParseObjectClass(int& tokIdx);
	MyObjectClassDefn* ParseObjectClassDefn(int& tokIdx);
	MyNamedNumberList* ParseNamedNumberList(int& tokIdx);
	MyNamedNumber* ParseNamedNumber(int& tokIdx);
	MyIdentifierList* ParseIdentifierList(int& tokIdx);
	MyNumericRealValue* ParseNumericRealValue(int& tokIdx);
	MySpecialRealValue* ParseSpecialRealValue(int& tokIdx);
	MyNamedValue* ParseNamedValue(int& tokIdx);
	MyComponentValueList* ParseComponentValueList(int& tokIdx);
	MyValueList* ParseValueList(int& tokIdx);
	MyNamedValueList* ParseNamedValueList(int& tokIdx);
	MyReferencedValue* ParseReferencedValue(int& tokIdx);
	MyValueFromObject* ParseValueFromObject(int& tokIdx);
	MyObjectClassFieldValue* ParseObjectClassFieldValue(int& tokIdx);
	MyOpenTypeFieldVal* ParseOpenTypeFieldVal(int& tokIdx);
	MyFixedTypeFieldVal* ParseFixedTypeFieldVal(int& tokIdx);
	MyObjectFromObject* ParseObjectFromObject(int& tokIdx);
	MyObjectSetFromObjects* ParseObjectSetFromObjects(int& tokIdx);
	MyLiteral* ParseLiteral(int& tokIdx);
	MyRequiredToken* ParseRequiredToken(int& tokIdx);
	MyOptionalGroup* ParseOptionalGroup(int& tokIdx);
	MyTokenOrGroupSpec* ParseTokenOrGroupSpec(int& tokIdx);
	MySyntaxList* ParseSyntaxList(int& tokIdx);
	MyWithSyntaxSpec* ParseWithSyntaxSpec(int& tokIdx);

	MyDefinedObject* ParseDefinedObject(int& tokIdx);
	MyDefinedObjectSet* ParseDefinedObjectSet(int& tokIdx);
	MyDefinedObjectClass* ParseDefinedObjectClass(int& tokIdx);
	MyExternalObjectClassReference* ParseExternalObjectClassReference(int& tokIdx);
	MyUsefulObjectClassReference* ParseUsefulObjectClassReference(int& tokIdx);
	MyFieldName* ParseFieldName(int& tokIdx);
	MyPrimitiveFieldName* ParsePrimitiveFieldName(int& tokIdx);
	MyExternalObjectReference* ParseExternalObjectReference(int& tokIdx);
	MyExternalObjectSetReference* ParseExternalObjectSetReference(int& tokIdx);
	MyParameterizedObject* ParseParameterizedObject(int& tokIdx);
	MyParameterizedObjectSet* ParseParameterizedObjectSet(int& tokIdx);
	MyParameterizedObjectClass* ParseParameterizedObjectClass(int& tokIdx);

	MyFieldSpec* ParseFieldSpec(int& tokIdx);
	MyTypeFieldSpec* ParseTypeFieldSpec(int& tokIdx);
	MyTypeOptionalitySpec* ParseTypeOptionalitySpec(int& tokIdx);
	MyValueOptionalitySpec* ParseValueOptionalitySpec(int& tokIdx);
	MyValueSetOptionalitySpec* ParseValueSetOptionalitySpec(int& tokIdx);
	MyObjectOptionalitySpec* ParseObjectOptionalitySpec(int& tokIdx);
	MyObjectSetOptionalitySpec* ParseObjectSetOptionalitySpec(int& tokIdx);
	MyFixedTypeValueFieldSpec* ParseFixedTypeValueFieldSpec(int& tokIdx);
	MyVariableTypeValueFieldSpec* ParseVariableTypeValueFieldSpec(int& tokIdx);
	MyFixedTypeValueSetFieldSpec* ParseFixedTypeValueSetFieldSpec(int& tokIdx);
	MyVariableTypeValueSetFieldSpec* ParseVariableTypeValueSetFieldSpec(int& tokIdx);
	MyObjectFieldSpec* ParseObjectFieldSpec(int& tokIdx);
	MyObjectSetFieldSpec* ParseObjectSetFieldSpec(int& tokIdx);
	bool ParseValueFieldReference(int& tokIdx, MyStringA* ret);
	bool ParseTypeFieldReference(int& tokIdx, MyStringA* ret);
	bool ParseValueSetFieldReference(int& tokIdx, MyStringA* ret);
	bool ParseObjectFieldReference(int& tokIdx, MyStringA* ret);
	bool ParseObjectSetFieldReference(int& tokIdx, MyStringA* ret);

	MyComponentTypeLists* ParseComponentTypeLists(int& tokIdx);
	MyComponentTypeList* ParseComponentTypeList(int& tokIdx);
	MyComponentType* ParseComponentType(int& tokIdx);

	MyTag* ParseTag(int& tokIdx);
	MyClassNumber* ParseClassNumber(int& tokIdx);
	MyClass* ParseClass(int& tokIdx);

	bool ParseObjectClassReference(int& tokIdx, MyStringA* ret);
	MyReference* ParseReference(int& tokIdx);
	MyGovernor* ParseGovernor(int& tokIdx);
	MyParamGovernor* ParseParamGovernor(int& tokIdx);
	MyParameter* ParseParameter(int& tokIdx);
	MyParameterList* ParseParameterList(int& tokIdx);

	MySetting* ParseSetting(int& tokIdx);
	MyFieldSetting* ParseFieldSetting(int& tokIdx);
	MyDefaultSyntax* ParseDefaultSyntax(int& tokIdx);
	MyDefinedSyntaxToken* ParseDefinedSyntaxToken(int& tokIdx);
	MyDefinedSyntax* ParseDefinedSyntax(int& tokIdx);

	bool IsToken(int tokIdx, int tokType, const char* token=NULL);
	int ExpectedTokenType(int& tokIdx, int tokType, const char* token=NULL);

private:
	MyModuleDef*       m_ModuleDef;
	MyArray<MyStringA> m_Tokens;
	MyValArray<int>    m_TokenTypes;

	MY_LAST_ERROR_DECL;
};


#endif // _MY_PARSER_H_

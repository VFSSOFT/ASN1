#ifndef _MY_PARSER_H_
#define _MY_PARSER_H_

#include "MyTokenizer.h"
#include "../notations/MyModule.h"
#include "../notations/MyTypeValueDef.h"
#include "../notations/MyRelativeObjectIDType.h"
#include "../notations/MyEnumeratedTypeDef.h"
#include "../notations/MyBitstringTypeDef.h"
#include "../notations/MyBoolTypeDef.h"
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

	MyAssignment* ParseAssignment(int& tokIdx);
	MyTypeAssignment* ParseTypeAssignment(int& tokIdx);
	MyValueAssignment* ParseValueAssignment(int& tokIdx);

	MyType* ParseType(int& tokIdx);
	MyBuiltinType* ParseBuiltinType(int& tokIdx);
	MyBitStringType* ParseBitStringType(int& tokIdx);
	MyNamedBitList* ParseNamedBitList(int& tokIdx);
	MyNamedBit* ParseNamedBit(int& tokIdx);
	MyBooleanType* ParseBoolType(int& tokIdx);
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
	MyValueSet* ParseValueSet(int& tokIdx);
	MyObject* ParseObject(int& tokIdx);
	MyObjectSet* ParseObjectSet(int& tokIdx);
	MyNamedNumberList* ParseNamedNumberList(int& tokIdx);
	MyNamedNumber* ParseNamedNumber(int& tokIdx);

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

	MyComponentTypeLists* ParseComponentTypeLists(int& tokIdx);
	MyComponentTypeList* ParseComponentTypeList(int& tokIdx);
	MyComponentType* ParseComponentType(int& tokIdx);

	MyTag* ParseTag(int& tokIdx);
	MyClassNumber* ParseClassNumber(int& tokIdx);
	MyClass* ParseClass(int& tokIdx);

	bool IsToken(int tokIdx, int tokType, const char* token=NULL);
	int ExpectedTokenType(int& tokIdx, int tokType, const char* token=NULL);

private:
	MyModuleDef*       m_ModuleDef;
	MyArray<MyStringA> m_Tokens;
	MyValArray<int>    m_TokenTypes;

	MY_LAST_ERROR_DECL;
};


#endif // _MY_PARSER_H_

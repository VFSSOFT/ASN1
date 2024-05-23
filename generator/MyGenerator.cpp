#ifndef _MY_GENERATOR_CPP_
#define _MY_GENERATOR_CPP_

#include "MyGenerator.h"

MyGenerator::MyGenerator(): m_LastErrorCode(0) {

}
MyGenerator::~MyGenerator() {

}

int MyGenerator::Process(MyModuleDef* modDef) {
	int err = 0;

	if (err = ProcessModuleId(modDef->ModuleId)) return err;
	if (err = ProcessTagDefault(modDef->TagDefault)) return err;

	if (modDef->Body) {
		if (modDef->Body->AssignmentList) {
			for (int i = 0; i < modDef->Body->AssignmentList->List.Size(); i++) {
				if (err = ProcessAssignment(modDef->Body->AssignmentList->List.Get(i))) return err;
			}
		}
	}

	return 0;
}

int MyGenerator::ProcessModuleId(MyModuleID* moduleId) {
	int err = 0;

	m_ModuleReference.Set(moduleId->ModuleReference.Deref());

	if (moduleId->DefinitiveID->Empty) {
		m_ModuleDefinitiveIdentifier.Reset();
	} else {
		// TODO:
		MyValArray<MyDefinitiveObjIdComponent*>* comps = &moduleId->DefinitiveID->DefinitiveObjIdComponentList->DefinitiveObjIdComponents;
		for (int i = 0; i < comps->Size(); i++) {
			MyDefinitiveObjIdComponent* c = comps->Get(i);
			if (c->NameForm) {
				
			} else if (c->DefinitiveNumberForm) {

			} else if (c->DefinitiveNameAndNumberForm) {

			}
		}
	}

	return 0;
}
int MyGenerator::ProcessTagDefault(MyTagDefault* tagDefault) {
	int err = 0;
	if (tagDefault->Value.Length() == 0) {
		m_DefaultTag.Set("EXPLICIT");
	} else {
		m_DefaultTag.Set(tagDefault->Value.Deref());
	}
	return 0;
}
int MyGenerator::ProcessAssignment(MyAssignment* ass) {
	int err = 0;

	if (ass->ValueAssignment) {
		if (err = ProcessValueAssignment(ass->ValueAssignment)) return err;
	} else if (ass->TypeAssignment) {
		if (err = ProcessTypeAssignment(ass->TypeAssignment)) return err;
	} else {
		assert(false);
	}

	return 0;
}
int MyGenerator::ProcessValueAssignment(MyValueAssignment* valAss) {
	int err = 0;
	MyValueAssignInfo* ass = new MyValueAssignInfo();
	
	ass->Name.Set(valAss->ValueReference.Deref());

	if (err = ProcessType(valAss->Type, &ass->Type)) return err;

	if (err = ProcessValue(valAss->Value, &ass->Value)) return err;

	m_ValueAssigns.Add(ass);
	return 0;
}
int MyGenerator::ProcessTypeAssignment(MyTypeAssignment* typAss) {
	int err = 0;

	MyTypeAssignInfo* ass = new MyTypeAssignInfo();

	ass->Name.Set(typAss->TypeReference.Deref());

	ass->Type = NULL;
	if (err = ProcessType(typAss->Type, &ass->Type)) return err;

	m_TypeAssigns.Add(ass);
	return 0;
}


int MyGenerator::ProcessType(MyType* typ, MyTypeInfo** retTypeInfo) {
	int err = 0;

	if (typ->BuiltinType) {
		if (err = ProcessBuiltinType(typ->BuiltinType, retTypeInfo)) return err;
	} else if (typ->ReferencedType) {
		if (err = ProcessReferenceType(typ->ReferencedType, retTypeInfo)) return err;
	} else if (typ->ConstrainedType) {
		if (err = ProcessConstrainedType(typ->ConstrainedType, retTypeInfo)) return err;
	} else {
		assert(false);
	}

	return 0;
}
int MyGenerator::ProcessBuiltinType(MyBuiltinType* typ, MyTypeInfo** retTypeInfo) {
	int err = 0;

	if (typ->BitStringType) {

	} else if (typ->BooleanType) {

	} else if (typ->CharStringType) {
		MyCharStringTypeInfo* typeInfo = new MyCharStringTypeInfo();
		if (typ->CharStringType->UnrestrictedCharStringType) {
			typeInfo->StrType.Set("");
		} else {
			typeInfo->StrType.Set(typ->CharStringType->RestrictedCharStringType->Type.DerefConst());
		}
		*retTypeInfo = typeInfo;
	} else if (typ->IntegerType) {
		*retTypeInfo = new MyIntegerTypeInfo();
	} else if (typ->NullType) {

	} else if (typ->ObjectClassFieldType) {

	} else if (typ->ObjectIDType) {
		*retTypeInfo = new MyOIDTypeInfo();
	} else if (typ->OctetStringType) {
		*retTypeInfo = new MyOctetStringTypeInfo();
	} else if (typ->RealType) {
  
	} else if (typ->SequenceType) {
		if (err = ProcessSequenceType(typ->SequenceType, retTypeInfo)) return err;
	} else if (typ->SequenceOfType) {
		if (err = ProcessSequenceOfType(typ->SequenceOfType, retTypeInfo)) return err;
	} else if (typ->SetType) {

	} else if (typ->SetOfType) {

	} else if (typ->TaggedType) {
		if (err = ProcessTaggedType(typ->TaggedType, retTypeInfo)) return err;
	} else {
		assert(false);
	}

	return 0;
}
int MyGenerator::ProcessReferenceType(MyReferencedType* typ, MyTypeInfo** retTypeInfo) {
	int err = 0;

	if (typ->DefinedType) {
		if (err = ProcessDefinedType(typ->DefinedType, retTypeInfo)) return err;
	} else if (typ->UsefulType.Length() > 0) {
		if (err = ProcessUsefulType(&typ->UsefulType, retTypeInfo)) return err;
	} else {
		assert(false);
	}

	return 0;
}
int MyGenerator::ProcessConstrainedType(MyConstrainedType* typ, MyTypeInfo** retTypeInfo) {
	int err = 0;

	if (typ->Type) {
		assert(typ->Type->BuiltinType != NULL);
		if (err = ProcessBuiltinType(typ->Type->BuiltinType, retTypeInfo)) return err;

		// Ignore the constraint
	} else if (typ->TypeWithConstraint) {
		MyTypeWithConstraint* typeconstraint = typ->TypeWithConstraint;
		if (typeconstraint->Set) {
			assert(false);
		} else {
			assert(typeconstraint->NamedType == NULL);
			MySequenceOfTypeInfo* typeInfo = new MySequenceOfTypeInfo();
			if (err = ProcessType(typeconstraint->Type, &typeInfo->Type)) return err;
			*retTypeInfo = typeInfo;
		}
	} else {
		assert(false);
	}

	return 0;
}
int MyGenerator::ProcessDefinedType(MyDefinedType* typ, MyTypeInfo** retTypeInfo) {
	MyRefTypeTypeInfo* refTypeTypeInfo = new MyRefTypeTypeInfo();
	if (typ->ExternalTypeReference) {
		refTypeTypeInfo->Module.Set(typ->ExternalTypeReference->ModuleReference.Deref());
		refTypeTypeInfo->Type.Set(typ->ExternalTypeReference->TypeReference.Deref());
	} else if (typ->TypeReference.Length() > 0) {
		refTypeTypeInfo->Type.Set(typ->TypeReference.Deref());
	} else {
		assert(false);
	}
	*retTypeInfo = refTypeTypeInfo;
	return 0;
}
int MyGenerator::ProcessSequenceType(MySequenceType* typ, MyTypeInfo** retTypeInfo) {
	int err = 0;

	MySequenceTypeInfo* seqTypeInfo = new MySequenceTypeInfo();

	if (typ->ComponentTypeLists->RootComponentTypeList) {
		MyValArray<MyComponentType*>* comps = &typ->ComponentTypeLists->RootComponentTypeList->ComponentTypes;
		for (int i = 0; i < comps->Size(); i++) {
			MyComponentType* compType = comps->Get(i);
			MyComponentTypeInfo* compInfo = new MyComponentTypeInfo();
			compInfo->Name.Set(compType->NamedType->Identifier.DerefConst());
			if (err = ProcessType(compType->NamedType->Type, &compInfo->Type)) return err;
			compInfo->Optional = compType->Optional;
			
			if (compType->DefaultValue) {
				if (err = ProcessValue(compType->DefaultValue, &compInfo->DefaultValue)) return err;
			} else {
				compInfo->DefaultValue = NULL;
			}

			seqTypeInfo->Components.Add(compInfo);
		}
	}

	*retTypeInfo = seqTypeInfo;
	return 0;
}
int MyGenerator::ProcessSequenceOfType(MySequenceOfType* typ, MyTypeInfo** retTypeInfo) {
	int err = 0;
	MySequenceOfTypeInfo* typeInfo = new MySequenceOfTypeInfo();

	if (typ->Type) {
		if (err = ProcessType(typ->Type, &typeInfo->Type)) return err;
	} else {
		assert(false);
	}
	
	*retTypeInfo = typeInfo;
	return 0;
}
int MyGenerator::ProcessTaggedType(MyTaggedType* typ, MyTypeInfo** retTypeInfo) {
	int err = 0;

	MyTaggedTypeInfo* typeInfo = new MyTaggedTypeInfo();
	typeInfo->Tag = typ->Tag;
	if (err = ProcessType(typ->Type, &typeInfo->Type)) return err;
	typeInfo->ImplicitOrExplicit.Set(typ->ImplicitOrExplicit.DerefConst());
  
	*retTypeInfo = typeInfo;
	return 0;
}
int MyGenerator::ProcessUsefulType(MyStringA* typ, MyTypeInfo** retTypeInfo) {
	MyRefTypeTypeInfo* typeInfo = new MyRefTypeTypeInfo();
	typeInfo->Type.Set(typ->Deref());
	*retTypeInfo = typeInfo;
	return 0;
}

int MyGenerator::ProcessValue(MyValue* val, MyValueInfo** retValueInfo) {
	int err = 0;

	if (val->BuiltinValue) {
		if (err = ProcessBuiltinValue(val->BuiltinValue, retValueInfo)) return err;
	} else {
		assert(false);
	}

	return 0;
}
int MyGenerator::ProcessBuiltinValue(MyBuiltinValue* val, MyValueInfo** retValueInfo) {
	int err = 0;

	if (val->BitStringValue) {

	} else if (val->BooleanValue) {

	} else if (val->ChoiceValue) {

	} else if (val->ExternalValue) {

	} else if (val->IntegerValue) {

	} else if (val->NullValue) {

	} else if (val->ObjectIDValue) {
		MyOIDValueInfo* oidValInfo = new MyOIDValueInfo();

		assert(val->ObjectIDValue->DefinedValue == NULL);

		for (int i = 0; i < val->ObjectIDValue->ObjIdComponentsList->ObjIdComponents.Size(); i++) {
			MyObjIdComponents* c = val->ObjectIDValue->ObjIdComponentsList->ObjIdComponents.Get(i);
			if (c->NameForm) {
				oidValInfo->Names.AddNew()->Set(c->NameForm->Deref());
			} else if (c->NumberForm) {
				oidValInfo->Numbers.AddNew()->Set(c->NumberForm->Number.DerefConst());
			} else if (c->NameAndNumberForm) {
				oidValInfo->Names.AddNew()->Set(c->NameAndNumberForm->Identifier.DerefConst());
				oidValInfo->Numbers.AddNew()->Set(c->NameAndNumberForm->NumberForm->Number.DerefConst());
			} else if (c->DefinedValue) {
				assert(false);
			}
		}

		*retValueInfo = oidValInfo;
	} else if (val->OctetStringValue) {

	} else if (val->RealValue) {

	} else if (val->SequenceValue) {

	} else if (val->SequenceOfValue) {

	} else if (val->SetValue) {

	} else if (val->SetOfValue) {

	} else if (val->TaggedValue) {

	} else {
		assert(false);
	}


	return 0;
}

#endif // _MY_GENERATOR_CPP_


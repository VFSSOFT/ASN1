#ifndef _MY_MODULE_H_
#define _MY_MODULE_H_

#include "MyNotationCommon.h"
#include "MyAssigningTypeValueDef.h"
#include "MyRelativeObjectIDType.h"
#include "MyParameterizaionDef.h"
#include "MyInformationObjectDef.h"

/*
Reference ::=
		typereference
		| valuereference
		| objectclassreference
		| objectreference
		| objectsetreference
*/
class MyReference : public NotationBase {
public:
	MyStringA TypeReference;
	MyStringA ValueReference;
	MyStringA ObjectClassReference;
	MyStringA ObjectReference;
	MyStringA ObjectSetReference;
};

/*
DefinitiveNumberForm ::= number
*/
class MyDefinitiveNumberForm : public NotationBase {
public:
	MyStringA Number;
};

/*
DefinitiveNameAndNumberForm ::= identifier "(" DefinitiveNumberForm ")"
*/
class MyDefinitiveNameAndNumberForm : public NotationBase {
public:
	MyStringA               Identifier;
	MyDefinitiveNumberForm* NumberForm;
};

/*
DefinitiveObjIdComponent ::=
		NameForm
		| DefinitiveNumberForm
		| DefinitiveNameAndNumberForm
*/
class MyDefinitiveObjIdComponent: public NotationBase {
public:
	MyNameForm*                    NameForm;
	MyDefinitiveNumberForm*        DefinitiveNumberForm;
	MyDefinitiveNameAndNumberForm* DefinitiveNameAndNumberForm;
};

/*
DefinitiveObjIdComponentList ::=
		DefinitiveObjIdComponent
		| DefinitiveObjIdComponent DefinitiveObjIdComponentList
*/
class MyDefinitiveObjIdComponentList: public NotationBase {
public:
	MyValArray<MyDefinitiveObjIdComponent*> DefinitiveObjIdComponents;
};


/*
DefinitiveIdentifier ::=
		"{" DefinitiveObjIdComponentList "}"
		| empty
*/
class MyDefinitiveID: public NotationBase {
public:
	MyDefinitiveObjIdComponentList* DefinitiveObjIdComponentList;
	bool                            Empty;
};

/*
ModuleIdentifier ::=
		modulereference
		DefinitiveIdentifier
*/
class MyModuleID: public NotationBase {
public:
	MyStringA       ModuleReference;
	MyDefinitiveID* DefinitiveID;
};

/*
TagDefault ::=
	EXPLICIT TAGS
	| IMPLICIT TAGS
	| AUTOMATIC TAGS
	| empty
*/

class MyTagDefault : public NotationBase {
public:
	MyStringA Value; // can be empty
};

/*
ExtensionDefault ::=
		EXTENSIBILITY IMPLIED
		| empty
*/
class MyExtensionDefault : public NotationBase {
public:
	MyStringA Value;
};

/*
Symbol ::=
		Reference
		| ParameterizedReference
*/
class MySymbol : public NotationBase {
public:
	MyReference*              Reference;
	MyParameterizedReference* ParameterizedReference;
};

/*
SymbolList ::=
		Symbol
		| SymbolList "," Symbol
*/
class MySymbolList : public NotationBase {
public:
	MyValArray<MySymbol*> Symbols;
};

/*
SymbolsExported ::=
		SymbolList
		| empty
*/
class MySymbolsExported : public NotationBase {
public:
	MySymbolList* Symbols;
	bool          Empty;
};

/*
Exports ::=
		EXPORTS SymbolsExported ";"
		| EXPORTS ALL ";"
		| empty
*/
class MyExports : public NotationBase {
public:
	MySymbolsExported* SymbolsExported;
	bool               ExportAll;
	bool               Empty;
};

/*
AssignedIdentifier ::=
		ObjectIdentifierValue
		| DefinedValue
		| empty
*/
class MyAssignedID : public NotationBase {
public:
	MyObjectIDValue* ObjectIdentifierValue;
	MyDefinedValue*  DefinedValue;
	bool             Empty;
};

/*
GlobalModuleReference ::=
		modulereference AssignedIdentifier
*/
class MyGlobalModuleReference : public NotationBase {
public:
	MyStringA     ModuleReference;
	MyAssignedID* AssignedID;
};

/*
SymbolsFromModule ::=
		SymbolList FROM GlobalModuleReference
*/
class MySymbolsFromModule : public NotationBase {
public:
	MySymbolList*            Symbols;
	MyGlobalModuleReference* GlobalModuleReference;
};

/*
SymbolsFromModuleList ::=
		SymbolsFromModule
		| SymbolsFromModuleList SymbolsFromModule
*/
class MySymbolsFromModuleList : public NotationBase {
public:
	MyValArray<MySymbolsFromModule*> List;
};

/*
SymbolsImported ::=
		SymbolsFromModuleList
		| empty
*/
class MySymbolsImported : public NotationBase {
public:
	MySymbolsFromModuleList* List;
	bool                     Empty;
};

/*
Imports ::=
		IMPORTS SymbolsImported ";"
		| empty
*/
class MyImports : public NotationBase {
public:
	MySymbolsImported* SymbolsImported;
	bool               Empty;
};


/*
Assignment ::=
		TypeAssignment
		| ValueAssignment
		| XMLValueAssignment
		| ValueSetTypeAssignment
		| ObjectClassAssignment
		| ObjectAssignment
		| ObjectSetAssignment
		| ParameterizedAssignment
*/
class MyAssignment : public NotationBase {
public:
	MyTypeAssignment*          TypeAssignment;
	MyValueAssignment*         ValueAssignment;
	MyXMLValueAssignment*      XMlValueAssignment;
	MyValueSetTypeAssignment*  ValueSetTypeAssignment;
	MyObjectClassAssignment*   ObjectClassAssignment;
	MyObjectAssignment*        ObjectAssignment;
	MyObjectSetAssignment*     ObjectSetAssignment;
	MyParameterizedAssignment* ParameterizedAssignment;
};

/*
AssignmentList ::=
		Assignment
		| AssignmentList Assignment
*/
class MyAssignmentList : public NotationBase {
public:
	MyValArray<MyAssignment*> List;
};

/*
ModuleBody ::=
		Exports Imports AssignmentList
		| empty
*/
class MyModuleBody : public NotationBase {
public:
	MyExports*        Exports;
	MyImports*        Imports;
	MyAssignmentList* AssignmentList;
	bool              Empty;
};


/*
ModuleDefinition ::=
		ModuleIdentifier
		DEFINITIONS
		TagDefault
		ExtensionDefault
		"::="
		BEGIN
		ModuleBody
		END
*/
class MyModuleDef: public NotationBase {
public:
	MyModuleID*         ModuleId;
	MyTagDefault*       TagDefault;
	MyExtensionDefault* ExtensionDefault;
	MyModuleBody*       Body;
};


#endif // _MY_MODULE_H_

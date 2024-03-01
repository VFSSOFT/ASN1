#ifndef _MY_MODULE_H_
#define _MY_MODULE_H_

#include "MyNotationCommon.h"


/*
DefinitiveNumberForm ::= number
*/
class MyDefinitiveNumberForm : public NotationBase {
public:
	MyBuffer Number;
};

/*
DefinitiveNameAndNumberForm ::= identifier "(" DefinitiveNumberForm ")"
*/
class MyDefinitiveNameAndNumberForm : public NotationBase {
public:
	MyStringA              Identifier;
	MyDefinitiveNumberForm NumberForm;
};

/*
DefinitiveObjIdComponent ::=
		NameForm
		| DefinitiveNumberForm
		| DefinitiveNameAndNumberForm
*/
class MyDefinitiveObjIdComponent: public NotationBase {
public:

};

/*
DefinitiveObjIdComponentList ::=
		DefinitiveObjIdComponent
		| DefinitiveObjIdComponent DefinitiveObjIdComponentList
*/
class MyDefinitiveObjIdComponentList: public MyArray<MyDefinitiveObjIdComponent>, public NotationBase {
};


/*
DefinitiveIdentifier ::=
		"{" DefinitiveObjIdComponentList "}"
		| empty
*/
class MyDefinitiveID: public NotationBase {
public:
	MyDefinitiveObjIdComponentList DefinitiveObjIdComponents;
	bool Empty;
};

/*
ModuleIdentifier ::=
		modulereference
		DefinitiveIdentifier
*/
class MyModuleID: public NotationBase {
public:
	MyDefinitiveID DefinitiveID;
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
SymbolList ::=
		Symbol
		| SymbolList "," Symbol
*/
class MySymbolList : public NotationBase {

};

/*
SymbolsExported ::=
		SymbolList
		| empty
*/
class MySymbolsExported : public NotationBase {
public:
	MySymbolList Symbols;
	bool         Empty;
};

/*
Exports ::=
		EXPORTS SymbolsExported ";"
		| EXPORTS ALL ";"
		| empty
*/
class MyExports : public NotationBase {
public:
	MySymbolsExported SymbolsExported;
	bool              ExportAll;
	bool              Empty;
};

/*
AssignedIdentifier ::=
		ObjectIdentifierValue
		| DefinedValue
		| empty
*/
class MyAssignedID : public NotationBase {
public:
	
	bool Empty;
};

/*
GlobalModuleReference ::=
		modulereference AssignedIdentifier
*/
class MyGlobalModuleReference : public NotationBase {
public:
	MyAssignedID AssignedID;
};

/*
SymbolsFromModule ::=
		SymbolList FROM GlobalModuleReference
*/
class MySymbolsFromModule : public NotationBase {
public:
	MySymbolList            Symbols;
	MyGlobalModuleReference GlobalModuleReference;
};

/*
SymbolsFromModuleList ::=
		SymbolsFromModule
		| SymbolsFromModuleList SymbolsFromModule
*/
class MySymbolsFromModuleList : public NotationBase {
public:
	MyArray<MySymbolsFromModule> List;
};

/*
SymbolsImported ::=
		SymbolsFromModuleList
		| empty
*/
class MySymbolsImported : public NotationBase {
public:
	MySymbolsFromModuleList List;
	bool                    Empty;
};

/*
Imports ::=
		IMPORTS SymbolsImported ";"
		| empty
*/
class MyImports : public NotationBase {
public:
	MySymbolsImported SymbolsImported;
	bool              Empty;
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

};

/*
AssignmentList ::=
		Assignment
		| AssignmentList Assignment
*/
class MyAssignmentList : public NotationBase {
public:
	MyArray<MyAssignment> List;
};

/*
ModuleBody ::=
		Exports Imports AssignmentList
		| empty
*/
class MyModuleBody : public NotationBase {
public:
	MyExports        Exports;
	MyImports        Imports;
	MyAssignmentList AssignmentList;
	bool             Empty;
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
	MyModuleID         ModuleId;
	MyTagDefault       TagDefault;
	MyExtensionDefault ExtensionDefault;
	MyModuleBody       Body;
};


#endif // _MY_MODULE_H_

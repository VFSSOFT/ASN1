#ifndef _MY_RELATIVE_OBJECT_ID_TYPE_H_
#define _MY_RELATIVE_OBJECT_ID_TYPE_H_

#include "MyNotationCommon.h"
#include "MyRefTypeValueDef.h"

class MyObjIdComponentsList;

/*
NameForm ::= identifier
*/
class MyNameForm : public NotationBase, public MyStringA {};

/*
XMLNumberForm ::= number
*/
class MyXMLNumberForm : public NotationBase, public MyStringA { };

/*
NumberForm ::= number | DefinedValue
*/
class MyNumberForm : public NotationBase {
public:
	MyStringA       Number;
	MyDefinedValue* DefinedValue;
};

/*
NameAndNumberForm ::=
	identifier "(" NumberForm ")"
*/
class MyNameAndNumberForm : public NotationBase {
public:
	MyStringA     Identifier;
	MyNumberForm* NumberForm;
};

/*
XMLNameAndNumberForm ::=
	identifier & "(" & XMLNumberForm & ")"
*/
class MyXMLNameAndNumberForm : public NotationBase {
public:
	MyStringA        Identifier;
	MyXMLNumberForm* XMLNumberForm;
};

/*
XMLObjectIdentifierValue ::=
	XMLObjIdComponentList
*/
class MyXMLObjectIDValue : public NotationBase {
public:

};


/*
XMLObjIdComponentList ::=
		XMLObjIdComponent
		| XMLObjIdComponent & "." & XMLObjIdComponentList
*/
class MyXMLObjIDComponentList : public NotationBase {
public:

};

/*
XMLObjIdComponent ::=
		NameForm
		| XMLNumberForm
		| XMLNameAndNumberForm
*/
class MyXMLObjIdComponent : public NotationBase {
public:

};

/*
ObjIdComponents ::=
	NameForm
	| NumberForm
	| NameAndNumberForm
	| DefinedValue
*/
class MyObjIdComponents : public NotationBase {
public:
	MyNameForm*          NameForm;
	MyNumberForm*        NumberForm;
	MyNameAndNumberForm* NameAndNumberForm;
	MyDefinedValue*      DefinedValue;
};

/*
ObjectIdentifierType ::=
		OBJECT IDENTIFIER
*/
class MyObjectIDType : public NotationBase {

};

/*
ObjectIdentifierValue ::=
		"{" ObjIdComponentsList "}"
		| "{" DefinedValue ObjIdComponentsList "}"
*/
class MyObjectIDValue : public NotationBase {
public:
	MyDefinedValue*        DefinedValue;
	MyObjIdComponentsList* ObjIdComponentsList;
};

/*
ObjIdComponentsList ::=
		ObjIdComponents
		| ObjIdComponents ObjIdComponentsList
*/
class MyObjIdComponentsList : public NotationBase {
public:
	MyValArray<MyObjIdComponents*> ObjIdComponents;
};






#endif // _MY_RELATIVE_OBJECT_ID_TYPE_H_

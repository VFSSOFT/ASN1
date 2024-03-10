#ifndef _MY_CHARACTER_STRING_TYPE_H_
#define _MY_CHARACTER_STRING_TYPE_H_


#include "MyNotationCommon.h"


class MyDefinedValue;
class MySequenceValue;

/*Group ::= number*/
class MyGroup: public NotationBase {
public:
	MyStringA Number;
};

/*Plane ::= number*/
class MyPlane: public NotationBase {
public:
	MyStringA Number;
};

/*Row ::= number*/
class MyRow : public NotationBase {
public:
	MyStringA Number;
};

/*Cell ::= number*/
class MyCell : public NotationBase {
public:
	MyStringA Number;
};

/*Quadruple ::= "{" Group "," Plane "," Row "," Cell "}"*/
class MyQuadruple : public NotationBase {
public:
	MyGroup* Group;
	MyPlane* Plane;
	MyRow*   Row;
	MyCell*  Cell;
};


/*TableColumn ::= number*/
class MyTableColumn : public NotationBase {
public:
	MyStringA Number;
};


/*TableRow ::= number*/
class MyTableRow : public NotationBase {
public:
	MyStringA Number;
};


/*Tuple ::= "{" TableColumn "," TableRow "}"*/
class MyTuple : public NotationBase {
public:
	MyTableColumn* TableCol;
	MyTableRow*    TableRow;
};

/*
CharsDefn ::=
		cstring
		| Quadruple
		| Tuple
		| DefinedValue
*/
class MyCharDefn: public NotationBase {
public:
	MyStringA       CString;
	MyQuadruple*    Quadruple;
	MyTuple*        Tuple;
	MyDefinedValue* DefinedValue;
};

/*
CharSyms ::=
		CharsDefn
		| CharSyms "," CharsDefn
*/
class MyCharSyms : public NotationBase {
public:
	MyValArray<MyCharDefn*> CharsDefn;
};

/*CharacterStringList ::= "{" CharSyms "}"*/
typedef MyCharSyms MyCharStringList;

/*
RestrictedCharacterStringValue ::=
		cstring
		| CharacterStringList
		| Quadruple
		| Tup
*/
class MyRestrictedCharStringValue : public NotationBase {
public:
	MyStringA CString;
	MyCharStringList* CharStringList;
	MyQuadruple*      Quadruple;
	MyTuple*          Tuple;
};

/*UnrestrictedCharacterStringType ::= CHARACTER STRING*/
class MyUnrestrictedChaStringType: public NotationBase {
public:
	MyStringA Value;
};

/*UnrestrictedCharacterStringValue ::= SequenceValue*/
class MyUnrestrictedCharacterStringValue : public NotationBase {
public:
	MySequenceValue* Value;
};

/*
RestrictedCharacterStringType ::=
		BMPString
		| GeneralString
		| GraphicString
		| IA5String
		| ISO646String
		| NumericString
		| PrintableString
		| TeletexString
		| T61String
		| UniversalString
		| UTF8String
		| VideotexString
		| VisibleString
*/
class MyRestrictedCharStringType : public NotationBase {
public:
	MyStringA Type;
};


/*
CharacterStringType ::=
		RestrictedCharacterStringType
		| UnrestrictedCharacterStringType
*/
class MyCharStringType : public NotationBase {
public:
	MyRestrictedCharStringType*  RestrictedCharStringType;
	MyUnrestrictedChaStringType* UnrestrictedCharStringType;
};


/*
CharacterStringValue ::=
		RestrictedCharacterStringValue
		| UnrestrictedCharacterStringValue
*/
class MyCharStringValue : public NotationBase {
public:
	MyRestrictedCharStringValue*        RestrictedCharStringValue;
	MyUnrestrictedCharacterStringValue* UnrestrictedCharStringValue;
};




#endif // _MY_CHARACTER_STRING_TYPE_H_


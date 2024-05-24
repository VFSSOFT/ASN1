#ifndef _MY_TYPE_INFO_H_
#define _MY_TYPE_INFO_H_

#include "../../common/MyStringA.h"

#include "MyValueInfo.h"

class MyTypeInfo {
public:
	char TypeInfoName[64];

	MyTypeInfo(const char* name) {
		strcpy(TypeInfoName, name);
	}

	bool IsInteger() { return STR_EQUALS(TypeInfoName, "Integer"); }
	bool IsBitString() { return STR_EQUALS(TypeInfoName, "BitString"); }
	bool IsOctetString() { return STR_EQUALS(TypeInfoName, "OctetString"); }
	bool IsCharString() { return STR_EQUALS(TypeInfoName, "CharString"); }
	bool IsRefType() { return STR_EQUALS(TypeInfoName, "RefType"); }
	bool IsSequence() { return STR_EQUALS(TypeInfoName, "Sequence"); }
	bool IsTagged() { return STR_EQUALS(TypeInfoName, "Tagged"); }
	bool IsSequenceOf() { return STR_EQUALS(TypeInfoName, "SequenceOf"); }
};

class MyOIDTypeInfo : public MyTypeInfo {
public:
	MyOIDTypeInfo() : MyTypeInfo("OID") {}
};

class MyOctetStringTypeInfo : public MyTypeInfo {
public:
	MyOctetStringTypeInfo() : MyTypeInfo("OctetString") {}
};

class MyBitStringTypeInfo : public MyTypeInfo {
public:
	MyBitStringTypeInfo() : MyTypeInfo("BitString") {}
};

class MyIntegerTypeInfo : public MyTypeInfo {
public:
	MyIntegerTypeInfo() : MyTypeInfo("Integer") {}
};

class MyCharStringTypeInfo : public MyTypeInfo {
public:
	MyStringA StrType;
	MyCharStringTypeInfo() : MyTypeInfo("CharString") {}
};

class MyTaggedTypeInfo : public MyTypeInfo {
public:
	MyTag*      Tag;
	MyTypeInfo* Type;
	MyStringA   ImplicitOrExplicit;

	MyTaggedTypeInfo() : MyTypeInfo("Tagged") {}
};

class MyRefTypeTypeInfo : public MyTypeInfo {
public:
	MyStringA Module; // ExternalTypeReference
	MyStringA Type;

	MyRefTypeTypeInfo() : MyTypeInfo("RefType") {}
};

class MyComponentTypeInfo : public MyTypeInfo {
public: 
	MyStringA    Name;
	MyTypeInfo*  Type;
	bool         Optional;
	MyValueInfo* DefaultValue;

	MyComponentTypeInfo() : MyTypeInfo("ComponentType") {}
};

class MySequenceTypeInfo : public MyTypeInfo {
public:
	MyValArray<MyComponentTypeInfo*> Components;

	MySequenceTypeInfo() : MyTypeInfo("Sequence") {}
};

class MySequenceOfTypeInfo : public MyTypeInfo {
public:
	MyTypeInfo* Type;
	//MyNamedType* NamedType;

	MySequenceOfTypeInfo() : MyTypeInfo("SequenceOf") {}
};

#endif // _MY_TYPE_INFO_H_

#ifndef _MY_TYPE_INFO_H_
#define _MY_TYPE_INFO_H_

#include "../../common/MyStringA.h"

#include "MyValueInfo.h"

class MyTypeInfo { };

class MyOIDTypeInfo : public MyTypeInfo { };

class MyIntegerTypeInfo : public MyTypeInfo {};

class MyCharStringTypeInfo : public MyTypeInfo {
public:
	MyStringA StrType;
};

class MyRefTypeTypeInfo : MyTypeInfo {
public:
	MyStringA Module; // ExternalTypeReference
	MyStringA Type;
};

class MyComponentTypeInfo : public MyTypeInfo {
public: 
	MyStringA    Name;
	MyTypeInfo*  Type;
	bool         Optional;
	MyValueInfo* DefaultValue;
};

class MySequenceTypeInfo : public MyTypeInfo {
public:
	MyValArray<MyComponentTypeInfo*> Components;
};

#endif // _MY_TYPE_INFO_H_

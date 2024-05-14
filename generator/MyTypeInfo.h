#ifndef _MY_TYPE_INFO_H_
#define _MY_TYPE_INFO_H_

#include "../../common/MyStringA.h"

class MyTypeInfo { };

class MyOIDTypeInfo : public MyTypeInfo { };

class MyIntegerTypeInfo : public MyTypeInfo {};

class MyCharStringTypeInfo : public MyTypeInfo {
public:
	MyStringA StrType;
};

#endif // _MY_TYPE_INFO_H_

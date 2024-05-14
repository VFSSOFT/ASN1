#ifndef _MY_ASSIGN_INFO_H_
#define _MY_ASSIGN_INFO_H_


#include "../../common/MyStringA.h"

#include "MyTypeInfo.h"
#include "MyValueInfo.h"

class MyTypeAssignInfo {
public:
	MyStringA   Name;
	MyTypeInfo* Type;
};

class MyValueAssignInfo {
public:
	MyStringA    Name;
	MyTypeInfo*  Type;
	MyValueInfo* Value;
};


#endif // _MY_ASSIGN_INFO_H_



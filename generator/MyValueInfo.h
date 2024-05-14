#ifndef _MY_VALUE_INFO_H_
#define _MY_VALUE_INFO_H_

#include "../../common/MyStringA.h"

class MyValueInfo {};

class MyOIDValueInfo : public MyValueInfo {
public:
	MyArray<MyStringA> Names;
	MyArray<MyStringA> Numbers;
};

#endif // _MY_VALUE_INFO_H_

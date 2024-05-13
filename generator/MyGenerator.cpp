#ifndef _MY_GENERATOR_CPP_
#define _MY_GENERATOR_CPP_

#include "MyGenerator.h"

MyGenerator::MyGenerator(): m_LastErrorCode(0), m_ModuleDef(NULL) {

}
MyGenerator::~MyGenerator() {

}

void MyGenerator::SetModule(MyModuleDef* modDef) {
	m_ModuleDef = modDef;
}


#endif // _MY_GENERATOR_CPP_


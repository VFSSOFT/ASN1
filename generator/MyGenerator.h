#ifndef _MY_GENERATOR_H_
#define _MY_GENERATOR_H_

#include "../notations/MyModule.h"

class MyGenerator {
public:

	MyGenerator();
	virtual ~MyGenerator();

	void SetModule(MyModuleDef* modDef);

private:
	MyModuleDef* m_ModuleDef;

	MY_LAST_ERROR_DECL;
};





#endif // _MY_GENERATOR_H_


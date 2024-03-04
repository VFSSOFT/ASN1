#ifndef _MY_NOTATION_COMMON_H_
#define _MY_NOTATION_COMMON_H_

#include "../../common/MyStringA.h"
#include "../../common/MyArray.h"
#include "../../common/MyBuffer.h"

class NotationBase {
public:
	NotationBase() : m_IsValid(false) {}

	bool IsValid() { return m_IsValid; }
	void SetValid(bool val) { m_IsValid = val; }

protected:
	bool m_IsValid;
};


#endif // _MY_NOTATION_COMMON_H_


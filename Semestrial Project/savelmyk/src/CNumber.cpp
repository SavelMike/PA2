// CNumber used to handle numbers of arbitrary precision

#include "CNumber.h"

ostream& operator <<(ostream& os, const CNumber* num)
{
	os << "output for CNumber";
	return os;
}

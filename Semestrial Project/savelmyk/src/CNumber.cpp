// CNumber used to handle numbers of arbitrary precision

#include "CNumber.h"

CNumber CNumber::operator +(const CNumber& a2) const 
{
	return CNumber(this->m_IntPart[0] + a2.m_IntPart[0]);
}

CNumber CNumber::operator -(const CNumber& a2) const 
{ 
	return CNumber(this->m_IntPart[0] - a2.m_IntPart[0]); 
}

CNumber CNumber::operator *(const CNumber& a2) const 
{ 
	return CNumber(this->m_IntPart[0] * a2.m_IntPart[0]); 
}

CNumber CNumber::operator /(const CNumber& a2) const 
{ 
	return CNumber(this->m_IntPart[0] / a2.m_IntPart[0]);
}

CNumber CNumber::operator %(const CNumber& a2) const 
{ 
	return CNumber(this->m_IntPart[0] % a2.m_IntPart[0]); 
}

ostream& operator <<(ostream& os, const CNumber& num)
{
	os << num.m_IntPart[0];
	return os;
}

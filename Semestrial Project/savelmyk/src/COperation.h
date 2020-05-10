// COperation handles binary expressions: +, -, *, /, %

#include "CNumber.h"

class COperation {
public:
	virtual CNumber operation(const CNumber* v1, const CNumber* v2)  const = 0;
};

class CAdd : public COperation {
public:
	CAdd() {}
	virtual CNumber operation(const CNumber* v1, const CNumber* v2) const { return *v1 + *v2; }
};

class CSub : public COperation {
public:
	CSub() {}
	virtual CNumber operation(const CNumber* v1, const CNumber* v2) const { return *v1 - *v2; }
};

class CMul : public COperation {
public:
	CMul() {}
	virtual CNumber operation(const CNumber* v1, const CNumber* v2) const { return *v1 * *v2; }
};

class CDiv : public COperation {
public:
	CDiv() {}
	virtual CNumber operation(const CNumber* v1, const CNumber* v2) const { return *v1 / *v2; }
};

class CMod : public COperation {
public:
	CMod() {}
	virtual CNumber operation(const CNumber* v1, const CNumber* v2) const { return *v1 % *v2; }
};
class CNone : public COperation {
public:
	CNone() {}
	virtual CNumber operation(const CNumber* v1, const CNumber* v2) const { return *v1; }
};
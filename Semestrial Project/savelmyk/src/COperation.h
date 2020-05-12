// COperation handles binary expressions: +, -, *, /, %

#ifndef COPERATION_H

#define COPERATION_H 

#include "CNumber.h"

class COperation {
public:
	virtual CNumber operation(const CNumber* v1, const CNumber* v2)  const { return *v1; }
	virtual bool isFactor() { return false; }
	virtual bool isExpr() { return false; }
};

class CAdd : public COperation {
public:
	CAdd() {}
	virtual CNumber operation(const CNumber* v1, const CNumber* v2) const { return *v1 + *v2; }
	virtual bool isFactor() { return false; }
	virtual bool isExpr() { return true; }
};

class CSub : public COperation {
public:
	CSub() {}
	virtual CNumber operation(const CNumber* v1, const CNumber* v2) const { return *v1 - *v2; }
	virtual bool isFactor() { return false; }
	virtual bool isExpr() { return true; }
};

class CMul : public COperation {
public:
	CMul() {}
	virtual CNumber operation(const CNumber* v1, const CNumber* v2) const { return *v1 * *v2; }
	virtual bool isFactor() { return true; }
	virtual bool isExpr() { return false; }
};

class CDiv : public COperation {
public:
	CDiv() {}
	virtual CNumber operation(const CNumber* v1, const CNumber* v2) const { return *v1 / *v2; }
	virtual bool isFactor() { return true; }
	virtual bool isExpr() { return false; }
};

class CMod : public COperation {
public:
	CMod() {}
	virtual CNumber operation(const CNumber* v1, const CNumber* v2) const { return *v1 % *v2; }
	virtual bool isFactor() { return true; }
	virtual bool isExpr() { return false; }
};
class CNone : public COperation {
public:
	CNone() {}
	virtual CNumber operation(const CNumber* v1, const CNumber* v2) const { return *v1; }
	virtual bool isFactor() { return false; }
	virtual bool isExpr() { return false; }
};


#endif

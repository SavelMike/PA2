#ifndef CADMIN_H

#define CADMIN_H

class CLexer;
// Base class
class CAdmin
{
public:
	virtual void cmd(CLexer& lex) { return; }
};

class CQuit : public CAdmin
{
public:
	virtual void cmd(CLexer& lex);
};

class CHistory : public CAdmin
{
public:
	virtual void cmd(CLexer& lex);
};

class CClear : public CAdmin
{
public:
	virtual void cmd(CLexer& lex);
};

class CHelp : public CAdmin
{
public:
	virtual void cmd(CLexer& lex);
};

class CVariables : public CAdmin
{
public:
	virtual void cmd(CLexer& lex);
};

class CRepeat : public CAdmin
{
public:
	virtual void cmd(CLexer& lex);
};

#endif
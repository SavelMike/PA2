#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */

// Define type for compare function
template <class T>
bool compare(const char *a, const T &b)
{
	return b == a;
}

/*
 * generic dynamic array
 */
template<class T>
class CDarrayT
{
private:
	int m_MaxSize; // Max number of elements in m_Data
	int m_CurSize; // Number of elements in array: in m_Data
	T* m_Data;
public:
	/* default constructor */
	CDarrayT();
	// Copy constructor
	CDarrayT(const CDarrayT<T>& orig);
	// Operator =
	CDarrayT<T>& operator = (const CDarrayT<T> & orig);
	~CDarrayT();
	void append(const T& t);
	const T& operator [](int idx) const;
  	T& operator [](int idx);
  	int find(const char *accID);

//	friend ostream & operator <<(ostream &os, const CDarrayT<T> &arr);
};

/*
 * Default constructor
 *   this make empty array of initial max size
 */
template<class T>
CDarrayT<T>::CDarrayT()
{
	m_MaxSize = 1;
	m_CurSize = 0;
	m_Data = new T[m_MaxSize];
}

/*
 * Copy constructor for dynamic array
 */
template<class T>
CDarrayT<T>::CDarrayT(const CDarrayT<T> & orig)
{
	this->m_MaxSize = orig.m_MaxSize;
	this->m_CurSize = orig.m_CurSize;
	this->m_Data = new T[this->m_MaxSize];
	for (int i = 0; i < orig.m_CurSize; i++) {
		this->m_Data[i] = orig.m_Data[i];
	}
}

/*
 * Destructor
 */
template<class T>
CDarrayT<T>::~CDarrayT()
{
	delete [] m_Data;
}

template<class T>
CDarrayT<T>& CDarrayT<T>::operator = (const CDarrayT<T> &orig)
{
	if (this == &orig)
		return *this;
	delete [] this->m_Data;
	this->m_MaxSize = orig.m_MaxSize;
	this->m_CurSize = orig.m_CurSize;
	this->m_Data = new T[this->m_MaxSize];
	for (int i = 0; i < orig.m_CurSize; i++) {
		this->m_Data[i] = orig.m_Data[i]; // Operator = for class T(CAccount or CTransaction)
	}
	return *this;
}

template<class T>
int CDarrayT<T>::find(const char *accID)
{
	for(int i = 0; i < m_CurSize; i++) {
		if (compare<T>(accID, m_Data[i]) == true) {
			return i;
		}
	}
	return -1;
}

/*
 * output (<<) operator friend for CDarrayT
 */
// ostream & operator <<(ostream &os, const CDarrayT &arr)
//{
//	for (int i = 0; i < arr.m_CurSize; i++)
//		os << setw(5) << arr.m_Data[i];
//	return os;
//}

/*
 * CDarrayT::append
 *   add one element to end of array
 *   allocate more memory if needed
 */
template<class T>
void CDarrayT<T>::append(const T& t)
{
	if (m_MaxSize == m_CurSize) {
		/* allocate more space */
		T* new_data = new T[m_MaxSize * 2];
		/* copy old data to new space */
		for (int i = 0; i < m_CurSize; i++)
			new_data[i] = m_Data[i];
		/* free old space, set data pointer to new space */
		delete [] m_Data;
		m_Data = new_data;
		m_MaxSize *= 2;
	}
	m_Data[m_CurSize] = t;
	m_CurSize++;
}

template<class T>
const T& CDarrayT<T>::operator[](int idx) const
{
	if (idx >= m_CurSize) {
		throw invalid_argument("too big index");
	}
	return m_Data[idx];
}

template <class T>
T& CDarrayT<T>::operator [](int idx)
{
	if (idx >= m_CurSize)
		throw invalid_argument("too big index 2");
	return m_Data[idx];
}

class CTransaction;

class CAccount
{
public:
	CAccount() { } // default constructor
	CAccount(const char *accid, int initbalance);
	int Balance() { return this->m_Balance; }
	const char * getaccID() const { return this->m_AccID; }
	void addTransaction(const CTransaction &transaction);
	void trimTransactions();
	bool operator == (const char *str) const;
	CAccount& operator =(const CAccount &orig);
	friend ostream & operator <<(ostream &os, const CAccount &acc);
private:
	char *m_AccID;
	int m_Balance;
	CDarrayT<CTransaction> m_Transactions;
};

CAccount::CAccount(const char *accid, int initialbalance)
:m_Transactions()
{
	this->m_Balance = initialbalance;
	this->m_AccID = new char[strlen(accid) + 1];
	strcpy(this->m_AccID, accid);
}

void CAccount::addTransaction(const CTransaction &transaction)
{
	this->m_Transactions.append(transaction);
}

void CAccount::trimTransactions()
{
	this->m_Transactions = CDarrayT<CTransaction>();
}

bool CAccount::operator ==(const char *str) const
{
	return strcmp(this->m_AccID, str);
}

CAccount& CAccount::operator =(const CAccount &orig)
{
	if (&orig == this)
		return *this;
	this->m_Balance = orig.m_Balance;
	delete [] this->m_AccID;
	this->m_AccID = new char[strlen(orig.m_AccID) + 1];
	strcpy(this->m_AccID, orig.m_AccID);
	this->m_Transactions = orig.m_Transactions; // Operator = for CDarrayT<CTransaction>

	return *this;
}

ostream & operator <<(ostream &os, const CAccount &acc)
{
	os << acc.m_AccID << acc.m_Balance;
	return os;
}

class CTransaction
{
public:
	CTransaction() { } // default constructor
	CTransaction(unsigned int amount, const char *signature);
	bool operator == (const char * str) const { return false; }

private:
	unsigned int m_Amount;
	char *m_Signature;
};

CTransaction::CTransaction(unsigned int amount, const char *signature)
{
	this->m_Amount = amount;
	this->m_Signature = new char [strlen(signature) + 1];
	strcpy(this->m_Signature, signature);
}



class CBank
{
public:
	// default constructor
	CBank():m_Accounts() { }
	// copy constructor
	CBank(const CBank &orig);
	// destructor
	~CBank();
	// operator =
	CBank & operator = (const CBank & orig);
	
    bool   NewAccount    ( const char * accID,
                           int          initialBalance );
    bool   Transaction   ( const char * debAccID,
                           const char * credAccID,
                           unsigned int amount,
                           const char * signature );
    bool   TrimAccount   ( const char * accID );
    CAccount & Account(const char *accID);

private:
    CDarrayT<CAccount> m_Accounts;
};

CBank::CBank(const CBank &orig)
{
	this->m_Accounts = orig.m_Accounts; // Operator = for CDarrayT<CAccounts>
}

CBank::~CBank() {
}

CBank& CBank::operator =(const CBank &orig)
{
	this->m_Accounts = orig.m_Accounts;
	return *this;
}

bool CBank::NewAccount(const char *accID, int initialBalance)
{
	CAccount acc(accID, initialBalance);
	this->m_Accounts.append(acc);
	return true;
}

bool CBank::Transaction(const char * debAccID, const char * credAccID, unsigned int amount, const char * signature)
{
	int debidx = this->m_Accounts.find(debAccID);
	if (debidx == -1) {
		return false;
	}
	int credidx = this->m_Accounts.find(credAccID);
	if (credidx == -1) {
		return false;
	}
	// Allocate new debit transaction
	CTransaction debt(amount, signature);
	this->m_Accounts[debidx].addTransaction(debt);

	// Allocate new credit transaction
	CTransaction credt(amount, signature);
	this->m_Accounts[credidx].addTransaction(credt);

	return true;
}

CAccount& CBank::Account(const char *accID)
{
	int idx = this->m_Accounts.find(accID);
	if (idx == -1)
		throw invalid_argument("Account not found");
	return this->m_Accounts[idx];
}

// TrimAccount(accID)
// the method releases the transactions associated with the account.
// In particular, the actual balance becomes the new initial balance and the list of transactions will be cleared.
// The method returns success(true) or error (false). An error is
// returned if the account does not exist in the database.
// Please note the method does not care about the payer's account balance.
bool CBank::TrimAccount(const char *accID)
{
	int idx = this->m_Accounts.find(accID);
	if (idx == -1) {
		return false;
	}
	this->m_Accounts[idx].trimTransactions();
	return true;
}

#ifndef __PROGTEST__
int main ( void )
{
  ostringstream os;
  char accCpy[100], debCpy[100], credCpy[100], signCpy[100];
  CBank x0;
  assert ( x0 . NewAccount ( "123456", 1000 ) );
  assert ( x0 . NewAccount ( "987654", -500 ) );
  assert ( x0 . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" ) );
  assert ( x0 . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" ) );
  assert ( x0 . NewAccount ( "111111", 5000 ) );
  assert ( x0 . Transaction ( "111111", "987654", 290, "Okh6e+8rAiuT5=" ) );
  assert ( x0 . Account ( "123456" ). Balance ( ) ==  -2190 );
  assert ( x0 . Account ( "987654" ). Balance ( ) ==  2980 );
  assert ( x0 . Account ( "111111" ). Balance ( ) ==  4710 );
  os . str ( "" );
  os << x0 . Account ( "123456" );
  assert ( ! strcmp ( os . str () . c_str (), "123456:\n   1000\n - 300, to: 987654, sign: XAbG5uKz6E=\n - 2890, to: 987654, sign: AbG5uKz6E=\n = -2190\n" ) );
  os . str ( "" );
  os << x0 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 290, from: 111111, sign: Okh6e+8rAiuT5=\n = 2980\n" ) );
  os . str ( "" );
  os << x0 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   5000\n - 290, to: 987654, sign: Okh6e+8rAiuT5=\n = 4710\n" ) );
  assert ( x0 . TrimAccount ( "987654" ) );
  assert ( x0 . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" ) );
  os . str ( "" );
  os << x0 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   2980\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 3103\n" ) );

  CBank x2;
  strncpy ( accCpy, "123456", sizeof ( accCpy ) );
  assert ( x2 . NewAccount ( accCpy, 1000 ));
  strncpy ( accCpy, "987654", sizeof ( accCpy ) );
  assert ( x2 . NewAccount ( accCpy, -500 ));
  strncpy ( debCpy, "123456", sizeof ( debCpy ) );
  strncpy ( credCpy, "987654", sizeof ( credCpy ) );
  strncpy ( signCpy, "XAbG5uKz6E=", sizeof ( signCpy ) );
  assert ( x2 . Transaction ( debCpy, credCpy, 300, signCpy ) );
  strncpy ( debCpy, "123456", sizeof ( debCpy ) );
  strncpy ( credCpy, "987654", sizeof ( credCpy ) );
  strncpy ( signCpy, "AbG5uKz6E=", sizeof ( signCpy ) );
  assert ( x2 . Transaction ( debCpy, credCpy, 2890, signCpy ) );
  strncpy ( accCpy, "111111", sizeof ( accCpy ) );
  assert ( x2 . NewAccount ( accCpy, 5000 ));
  strncpy ( debCpy, "111111", sizeof ( debCpy ) );
  strncpy ( credCpy, "987654", sizeof ( credCpy ) );
  strncpy ( signCpy, "Okh6e+8rAiuT5=", sizeof ( signCpy ) );
  assert ( x2 . Transaction ( debCpy, credCpy, 2890, signCpy ) );
  assert ( x2 . Account ( "123456" ). Balance ( ) ==  -2190 );
  assert ( x2 . Account ( "987654" ). Balance ( ) ==  5580 );
  assert ( x2 . Account ( "111111" ). Balance ( ) ==  2110 );
  os . str ( "" );
  os << x2 . Account ( "123456" );
  assert ( ! strcmp ( os . str () . c_str (), "123456:\n   1000\n - 300, to: 987654, sign: XAbG5uKz6E=\n - 2890, to: 987654, sign: AbG5uKz6E=\n = -2190\n" ) );
  os . str ( "" );
  os << x2 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n = 5580\n" ) );
  os . str ( "" );
  os << x2 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n = 2110\n" ) );
  assert ( x2 . TrimAccount ( "987654" ) );
  strncpy ( debCpy, "111111", sizeof ( debCpy ) );
  strncpy ( credCpy, "987654", sizeof ( credCpy ) );
  strncpy ( signCpy, "asdf78wrnASDT3W", sizeof ( signCpy ) );
  assert ( x2 . Transaction ( debCpy, credCpy, 123, signCpy ) );
  os . str ( "" );
  os << x2 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   5580\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 5703\n" ) );

  CBank x4;
  assert ( x4 . NewAccount ( "123456", 1000 ) );
  assert ( x4 . NewAccount ( "987654", -500 ) );
  assert ( !x4 . NewAccount ( "123456", 3000 ) );
  assert ( !x4 . Transaction ( "123456", "666", 100, "123nr6dfqkwbv5" ) );
  assert ( !x4 . Transaction ( "666", "123456", 100, "34dGD74JsdfKGH" ) );
  assert ( !x4 . Transaction ( "123456", "123456", 100, "Juaw7Jasdkjb5" ) );
  try
  {
    x4 . Account ( "666" ). Balance ( );
    assert ( "Missing exception !!" == NULL );
  }
  catch ( ... )
  {
  }
  try
  {
    os << x4 . Account ( "666" ). Balance ( );
    assert ( "Missing exception !!" == NULL );
  }
  catch ( ... )
  {
  }
  assert ( !x4 . TrimAccount ( "666" ) );

  CBank x6;
  assert ( x6 . NewAccount ( "123456", 1000 ) );
  assert ( x6 . NewAccount ( "987654", -500 ) );
  assert ( x6 . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" ) );
  assert ( x6 . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" ) );
  assert ( x6 . NewAccount ( "111111", 5000 ) );
  assert ( x6 . Transaction ( "111111", "987654", 2890, "Okh6e+8rAiuT5=" ) );
  CBank x7 ( x6 );
  assert ( x6 . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" ) );
  assert ( x7 . Transaction ( "111111", "987654", 789, "SGDFTYE3sdfsd3W" ) );
  assert ( x6 . NewAccount ( "99999999", 7000 ) );
  assert ( x6 . Transaction ( "111111", "99999999", 3789, "aher5asdVsAD" ) );
  assert ( x6 . TrimAccount ( "111111" ) );
  assert ( x6 . Transaction ( "123456", "111111", 221, "Q23wr234ER==" ) );
  os . str ( "" );
  os << x6 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   -1802\n + 221, from: 123456, sign: Q23wr234ER==\n = -1581\n" ) );
  os . str ( "" );
  os << x6 . Account ( "99999999" );
  assert ( ! strcmp ( os . str () . c_str (), "99999999:\n   7000\n + 3789, from: 111111, sign: aher5asdVsAD\n = 10789\n" ) );
  os . str ( "" );
  os << x6 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 5703\n" ) );
  os . str ( "" );
  os << x7 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n - 789, to: 987654, sign: SGDFTYE3sdfsd3W\n = 1321\n" ) );
  try
  {
    os << x7 . Account ( "99999999" ). Balance ( );
    assert ( "Missing exception !!" == NULL );
  }
  catch ( ... )
  {
  }
  os . str ( "" );
  os << x7 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n + 789, from: 111111, sign: SGDFTYE3sdfsd3W\n = 6369\n" ) );

  CBank x8;
  CBank x9;
  assert ( x8 . NewAccount ( "123456", 1000 ) );
  assert ( x8 . NewAccount ( "987654", -500 ) );
  assert ( x8 . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" ) );
  assert ( x8 . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" ) );
  assert ( x8 . NewAccount ( "111111", 5000 ) );
  assert ( x8 . Transaction ( "111111", "987654", 2890, "Okh6e+8rAiuT5=" ) );
  x9 = x8;
  assert ( x8 . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" ) );
  assert ( x9 . Transaction ( "111111", "987654", 789, "SGDFTYE3sdfsd3W" ) );
  assert ( x8 . NewAccount ( "99999999", 7000 ) );
  assert ( x8 . Transaction ( "111111", "99999999", 3789, "aher5asdVsAD" ) );
  assert ( x8 . TrimAccount ( "111111" ) );
  os . str ( "" );
  os << x8 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   -1802\n = -1802\n" ) );
  os . str ( "" );
  os << x9 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n - 789, to: 987654, sign: SGDFTYE3sdfsd3W\n = 1321\n" ) );

  return 0;
}

#endif /* __PROGTEST__ */

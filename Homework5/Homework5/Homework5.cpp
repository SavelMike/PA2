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
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
#endif /* __PROGTEST */
using namespace std;

#ifndef __PROGTEST__

// CTimeStamp
// Is a simple container class that represent a time stamp (date and time).
// This class is implemented in the testing environment, you will have to implement its lightweight variant for your testing.
// Important: the declaration and the implementation of your "mocked" variant must be kept
// in the conditional compile block (as shown in the attached file).
class CTimeStamp
{
public:
    CTimeStamp(int               year,
        int               month,
        int               day,
        int               hour,
        int               minute,
        int               sec);
    int            Compare(const CTimeStamp& x) const;
    friend ostream& operator <<                           (ostream& os,
        const CTimeStamp& x);
private:
    int m_Year;
    int m_Month;
    int m_Day;
    int m_Hour;
    int m_Minute;
    int m_Sec;
};

// constructor CTimeStamp class
CTimeStamp::CTimeStamp(int year, int month, int day, int hour,
    int minute, int sec)
{
    this->m_Year = year;
    this->m_Month = month;
    this->m_Day = day;
    this->m_Hour = hour;
    this->m_Minute = minute;
    this->m_Sec = sec;
}

// Compare (x)
// The method compares the instance and parameter x.
// Return value is a negative number (instance is smaller than x), zero (instances represent identical time stamps),
// or positive (the instance is bigger than x).
int CTimeStamp::Compare(const CTimeStamp& x) const
{
    int diff;
    diff = this->m_Year - x.m_Year;
    if (diff != 0) {
        return diff;
    }
    diff = this->m_Month - x.m_Month;
    if (diff != 0) {
        return diff;
    }
    diff = this->m_Day - x.m_Day;
    if (diff != 0) {
        return diff;
    }
    diff = this->m_Hour - x.m_Hour;
    if (diff != 0) {
        return diff;
    }
    diff = this->m_Minute - x.m_Minute;
    if (diff != 0) {
        return diff;
    }
    return this->m_Sec - x.m_Sec;
}

// output operator
// displays the time stamp in format YYYY-MM-DD HH24:MI:SS.
ostream& operator << (ostream& os, const CTimeStamp& x)
{
    os << setw(4) << setfill('0') << x.m_Year << "-" << setw(2) << x.m_Month << "-" << x.m_Day << " "
        << x.m_Hour << ":" << x.m_Minute << ":" << x.m_Sec;
    return os;
}

//=================================================================================================
// CMailBody
// Is another container, the class represents mail body.
// Again, this class is implemented in the testing environment, you will have to implement
// its lightweight variant for your testing.
// Important: the declaration and the implementation of your "mocked" variant must be
// kept in the conditional compile block (as shown in the attached file).
class CMailBody
{
public:
    CMailBody(int size, const char* data);
    // copy cons/op=/destructor is correctly implemented in the testing environment
    CMailBody(const CMailBody& orig); // Copy constructor
    ~CMailBody();
    CMailBody& operator = (const CMailBody& orig);

    friend ostream& operator << (ostream& os, const CMailBody& x)
    {
        return os << "mail body: " << x.m_Size << " B";
    }
private:
    int            m_Size;
    char* m_Data;
};

CMailBody::CMailBody(int size, const char* data)
{
    this->m_Size = size;
    this->m_Data = new char[size];
    memcpy(this->m_Data, data, size);
}

CMailBody::CMailBody(const CMailBody& orig)
{
    this->m_Size = orig.m_Size;
    this->m_Data = new char[orig.m_Size];
    memcpy(this->m_Data, orig.m_Data, orig.m_Size);
}

CMailBody::~CMailBody()
{
    delete[] this->m_Data;
}

CMailBody& CMailBody::operator = (const CMailBody& orig)
{
    if (this == &orig) {
        return *this;
    }
    delete[] m_Data;
    this->m_Size = orig.m_Size;
    this->m_Data = new char[orig.m_Size];
    memcpy(this->m_Data, orig.m_Data, orig.m_Size);

    return *this;
}



//=================================================================================================
// CAttach
// This class represents mail attachment.
// The class is implemented in the testing environment, moreover, a lightweight implementation is present in the attached file.
// It is important to keep the implementation in the conditional compile block (as in the attached file).
class CAttach
{
public:
    // constructor
    // creates an instance based on the contents (simplified).
    // Next, the constructor initializes m_RefCnt, this member counts the references.
    // The instance will be deleted when the number of references drops to 0 (m_RefCnt==0).
    CAttach(int x)
        : m_X(x),
        m_RefCnt(1)
    {

    }
    // AddRef
    // This method increases the number of references.
    // When a reference to the object is created (such as when the constructor of CMail stores the pointer
    // or when getter Attach returns the reference),
    // this method is to be called to increase the counter to match the number of actual references.
    void AddRef(void) const
    {
        m_RefCnt++;
    }

    // Release
    // The method decreases the reference count. When the reference count drops to zero, the instance commits a suicide.
    // The method will be when a reference to the instance is about to vanish, such as in the destructor of CMail. Similarly,
    // Release needs to be called when a reference to attachment object (previously obtained via getter Attach) is to vanish.
    void Release(void) const
    {
        if (!--m_RefCnt)
            delete this;
    }
private:
    int            m_X;

    // mutable m_RefCnt
    // the mutable keyword allows modifications of m_RefCnt even from const methods.
    mutable int    m_RefCnt;
    // copy constructor, operator =, ...
    // these are private, thus cannot be used by other parts of the program.
    CAttach(const CAttach& x);
    CAttach& operator = (const CAttach& x);

    // destructor
    // releases the resources allocated in the instance (simplified in the delivered implementation).
    // Please note the destructor is private. The private destructor means two things.
    // First, instances of CAttach cannot be allocated statically. Second, instances of CAttach cannot be deleted directly by delete. There is a good reason: an instance (possibly referenced from many other objects) cannot be deleted.
    // Instead, the AddRef/Release mechanism must be used (thus the users of CAttach must follow the counted references).
    ~CAttach(void) = default;
    friend ostream& operator <<                           (ostream& os,
        const CAttach& x)
    {
        return os << "attachment: " << x.m_X << " B";
    }
};

// constructor
// creates an instance based on the contents (simplified).
// Next, the constructor initializes m_RefCnt, this member counts the references.
// The instance will be deleted when the number of references drops to 0 (m_RefCnt==0).
CAttach::CAttach(const CAttach& x)
{
    this->m_RefCnt = 1;
    this->m_X = 1;
}


CAttach& CAttach::operator =(const CAttach& x)
{
    this->m_RefCnt = x.m_RefCnt;
    this->m_X = x.m_X;
    return *this;
}
//=================================================================================================
#endif /* __PROGTEST__, DO NOT remove */

// CMail
// This class represents one e-mail.
// The e-mail is represented by the date/time stamp, the address of the sender, the body of the mail, and an optional attachment.
// To represent these components, your implementation shall use classes CTimeStamp, std::string, CMailBody, and CAttach, respectively.
class CMail
{
public:
    CMail(const CTimeStamp& timeStamp,
        const string& from,
        const CMailBody& body,
        const CAttach* attach);
    const string& From(void) const;
    const CMailBody& Body(void) const;
    const CTimeStamp& TimeStamp(void) const;
    const CAttach* Attachment(void) const;

    friend ostream& operator <<                           (ostream& os,
        const CMail& x);
private:
    CTimeStamp m_TimeStamp;
    string m_From;
    CMailBody m_Body;
    const CAttach* m_Attach;
};

// constructor
// creates an instance of e-mail based on the parameters.
// Parameter attach may be NULL indicating an e-mail without an attachment,
CMail::CMail(const CTimeStamp& timeStamp, const string& from,
    const CMailBody& body, const CAttach* attach)
    :m_TimeStamp(timeStamp), m_From(from), m_Body(body)
{
    this->m_Attach = attach;
}

// output operator
// displays the time stamp, sender, mail body and (optionally) the attachment.
// The objects representing individual fields do support output operators,
// the fields are to be separated by a single space (with an exception for the attachment,
// see the exact format in the enclosed archive).
// Indeed, the operator is not tested in the testing environment, the purpose of the operator is to simplify testing.
ostream& operator << (ostream& os, const CMail& x)
{
    os << x.m_TimeStamp << " " << x.m_From << " " << x.m_Body << " " << *x.m_Attach;
    return os;
}


const string& CMail::From(void) const
{
    return this->m_From;
}

const CMailBody& CMail::Body(void) const
{
    return this->m_Body;
}

const CTimeStamp& CMail::TimeStamp(void) const
{
    return this->m_TimeStamp;
}

const CAttach* CMail::Attachment(void) const
{
    return this->m_Attach;
}


class CFolder
{
public:
    CFolder(const string& foldername)
        :m_Foldername(foldername), m_Mails() { }
    bool addMail(const CMail& mail);
private:
    vector<CMail> m_Mails;
    string m_Foldername;
};

bool CFolder::addMail(const CMail& mail)
{
    this->m_Mails.push_back(mail);
    return true;
}

//=================================================================================================
// CMailBox
// This class represents one mail box.
// The mail box holds incoming mail (i.e. instances of CMail), the e-mails can be organized into folders.
// The CMailBox class is to be developed by you.

class CMailBox
{
public:
    CMailBox(void);
    bool           Delivery(const CMail& mail);
    bool           NewFolder(const string& folderName);
    bool           MoveMail(const string& fromFolder,
                            const string& toFolder);
    list<CMail>    ListMail(const string& folderName,
                            const CTimeStamp& from,
                            const CTimeStamp& to) const;
    set<string>    ListAddr(const CTimeStamp& from,
                            const CTimeStamp& to) const;
private:
    vector<CFolder> m_Folders;
};

// constructor
//	creates an empty mailbox (no mail). Moreover, the new mailbox will contain one folder inbox.
CMailBox::CMailBox()
    :m_Folders() // Create an empty vector
{
    CFolder folder("inbox");
    this->m_Folders.push_back(folder);
}

// Delivery (mail)
// The method will be called when a new e-mail is to be stored into the mailbox.
// The new mail will be copied into the inbox folder. The method returns true (success),
// or false (failure, however, delivery shall not fail).
bool CMailBox::Delivery(const CMail& mail)
{
    return true;
}

// NewFolder (name)
// The method creates a new folder named name.
// The newly created folder shall be empty.
// The method returns true (success), or false (failure, folder of the name already exists).
// We assume one-level hierarchy of folders, i.e. folders cannot be nested.
// 		There are no restrictions on folder names.
bool CMailBox::NewFolder(const string& folderName)
{
    return true;
}

// MoveMail (fromFld, toFld)
// The method moves all e-mails from folder fromFld into folder toFld.
// The e-mails are moved, i.e. fromFld will be empty after a successful completion.
// The method returns true (success), or false (failure, from/to folder does not exist).
bool CMailBox::MoveMail(const string& fromFolder,
    const string& toFolder)
{
    return true;
}

// ListMail (fld, from, to)
// The method returns a list of e-mails selected from folder fld, moreover,
// the e-mails will be returned if the e-mail time stamp fits the time interval (from - to, both inclusive).
// If the folder does not exist, the method returns an empty list.
list<CMail> CMailBox::ListMail(const string& folderName,
    const CTimeStamp& from, const CTimeStamp& to) const
{
    list<CMail> foolist;
    return foolist;
}

// ListAddr (from, to)
// The method returns a set of e-mail addresses.
// The addresses will be selected from the e-mail that fits into the interval (from - to, both inclusive).
// The method shall search all folders.
// If no mail fits the time interval, the returned set will be empty.
set<string> CMailBox::ListAddr(const CTimeStamp& from,
    const CTimeStamp& to) const
{
    set<string> list;

    return list;
}

//=================================================================================================
#ifndef __PROGTEST__
static string showMail(const list<CMail>& l)
{
    ostringstream oss;
    for (const auto& x : l)
        oss << x << endl;
    return oss.str();
}
static string showUsers(const set<string>& s)
{
    ostringstream oss;
    for (const auto& x : s)
        oss << x << endl;
    return oss.str();
}
int main(void)
{
    list<CMail> mailList;
    set<string> users;
    CAttach* att;

    CMailBox m0;
    assert(m0.Delivery(CMail(CTimeStamp(2014, 3, 31, 15, 24, 13), "user1@fit.cvut.cz", CMailBody(14, "mail content 1"), nullptr)));
    assert(m0.Delivery(CMail(CTimeStamp(2014, 3, 31, 15, 26, 23), "user2@fit.cvut.cz", CMailBody(22, "some different content"), nullptr)));
    att = new CAttach(200);
    assert(m0.Delivery(CMail(CTimeStamp(2014, 3, 31, 11, 23, 43), "boss1@fit.cvut.cz", CMailBody(14, "urgent message"), att)));
    assert(m0.Delivery(CMail(CTimeStamp(2014, 3, 31, 18, 52, 27), "user1@fit.cvut.cz", CMailBody(14, "mail content 2"), att)));
    att->Release();
    att = new CAttach(97);
    assert(m0.Delivery(CMail(CTimeStamp(2014, 3, 31, 16, 12, 48), "boss1@fit.cvut.cz", CMailBody(24, "even more urgent message"), att)));
    att->Release();
    assert(showMail(m0.ListMail("inbox",
        CTimeStamp(2000, 1, 1, 0, 0, 0),
        CTimeStamp(2050, 12, 31, 23, 59, 59))) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B + attachment: 200 B\n"
        "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
        "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B\n"
        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B + attachment: 97 B\n"
        "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 200 B\n");
    assert(showMail(m0.ListMail("inbox",
        CTimeStamp(2014, 3, 31, 15, 26, 23),
        CTimeStamp(2014, 3, 31, 16, 12, 48))) == "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B\n"
        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B + attachment: 97 B\n");
    assert(showUsers(m0.ListAddr(CTimeStamp(2000, 1, 1, 0, 0, 0),
        CTimeStamp(2050, 12, 31, 23, 59, 59))) == "boss1@fit.cvut.cz\n"
        "user1@fit.cvut.cz\n"
        "user2@fit.cvut.cz\n");
    assert(showUsers(m0.ListAddr(CTimeStamp(2014, 3, 31, 15, 26, 23),
        CTimeStamp(2014, 3, 31, 16, 12, 48))) == "boss1@fit.cvut.cz\n"
        "user2@fit.cvut.cz\n");

    CMailBox m1;
    assert(m1.NewFolder("work"));
    assert(m1.NewFolder("spam"));
    assert(!m1.NewFolder("spam"));
    assert(m1.Delivery(CMail(CTimeStamp(2014, 3, 31, 15, 24, 13), "user1@fit.cvut.cz", CMailBody(14, "mail content 1"), nullptr)));
    att = new CAttach(500);
    assert(m1.Delivery(CMail(CTimeStamp(2014, 3, 31, 15, 26, 23), "user2@fit.cvut.cz", CMailBody(22, "some different content"), att)));
    att->Release();
    assert(m1.Delivery(CMail(CTimeStamp(2014, 3, 31, 11, 23, 43), "boss1@fit.cvut.cz", CMailBody(14, "urgent message"), nullptr)));
    att = new CAttach(468);
    assert(m1.Delivery(CMail(CTimeStamp(2014, 3, 31, 18, 52, 27), "user1@fit.cvut.cz", CMailBody(14, "mail content 2"), att)));
    att->Release();
    assert(m1.Delivery(CMail(CTimeStamp(2014, 3, 31, 16, 12, 48), "boss1@fit.cvut.cz", CMailBody(24, "even more urgent message"), nullptr)));
    assert(showMail(m1.ListMail("inbox",
        CTimeStamp(2000, 1, 1, 0, 0, 0),
        CTimeStamp(2050, 12, 31, 23, 59, 59))) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
        "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
        "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
        "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n");
    assert(showMail(m1.ListMail("work",
        CTimeStamp(2000, 1, 1, 0, 0, 0),
        CTimeStamp(2050, 12, 31, 23, 59, 59))) == "");
    assert(m1.MoveMail("inbox", "work"));
    assert(showMail(m1.ListMail("inbox",
        CTimeStamp(2000, 1, 1, 0, 0, 0),
        CTimeStamp(2050, 12, 31, 23, 59, 59))) == "");
    assert(showMail(m1.ListMail("work",
        CTimeStamp(2000, 1, 1, 0, 0, 0),
        CTimeStamp(2050, 12, 31, 23, 59, 59))) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
        "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
        "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
        "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n");
    assert(m1.Delivery(CMail(CTimeStamp(2014, 3, 31, 19, 24, 13), "user2@fit.cvut.cz", CMailBody(14, "mail content 4"), nullptr)));
    att = new CAttach(234);
    assert(m1.Delivery(CMail(CTimeStamp(2014, 3, 31, 13, 26, 23), "user3@fit.cvut.cz", CMailBody(9, "complains"), att)));
    att->Release();
    assert(showMail(m1.ListMail("inbox",
        CTimeStamp(2000, 1, 1, 0, 0, 0),
        CTimeStamp(2050, 12, 31, 23, 59, 59))) == "2014-03-31 13:26:23 user3@fit.cvut.cz mail body: 9 B + attachment: 234 B\n"
        "2014-03-31 19:24:13 user2@fit.cvut.cz mail body: 14 B\n");
    assert(showMail(m1.ListMail("work",
        CTimeStamp(2000, 1, 1, 0, 0, 0),
        CTimeStamp(2050, 12, 31, 23, 59, 59))) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
        "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
        "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
        "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n");
    assert(m1.MoveMail("inbox", "work"));
    assert(showMail(m1.ListMail("inbox",
        CTimeStamp(2000, 1, 1, 0, 0, 0),
        CTimeStamp(2050, 12, 31, 23, 59, 59))) == "");
    assert(showMail(m1.ListMail("work",
        CTimeStamp(2000, 1, 1, 0, 0, 0),
        CTimeStamp(2050, 12, 31, 23, 59, 59))) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
        "2014-03-31 13:26:23 user3@fit.cvut.cz mail body: 9 B + attachment: 234 B\n"
        "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
        "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
        "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n"
        "2014-03-31 19:24:13 user2@fit.cvut.cz mail body: 14 B\n");

    return 0;
}

#endif /* __PROGTEST__ */



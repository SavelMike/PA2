#if 0
#ifndef __PROGTEST__
#include <cctype>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <set>
#include <list>
#include <forward_list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <deque>
#include <optional>
#include <variant>
#include <any>
#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

template <typename _Type, typename _Comparator = ... /* todo */ >
class CSearch
{
public:
    // default constructor
    // constructor with comparator parameter
    // destructor (if needed)
    CSearch(const CSearch& src) = delete;
    CSearch& operator = (const CSearch& src) = delete;
    void      Add(int           id,
        const _Type& needle);
    set<int>  Search(const _Type& hayHeap) const;
private:
    // todo
};
//-------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__
class CharComparator
{
public:
    CharComparator(bool caseSensitive = true)
        : m_CaseSensitive(caseSensitive)
    {
    }
    bool       operator () (const char& a, const char& b) const
    {
        return m_CaseSensitive ? a == b : toupper(a) == toupper(b);
    }
private:
    bool       m_CaseSensitive;
};
//-------------------------------------------------------------------------------------------------
bool upperCaseCompare(const char& a, const char& b)
{
    return toupper(a) == toupper(b);
}
//-------------------------------------------------------------------------------------------------
int main(void)
{
    CSearch <string> test1;
    test1.Add(0, "hello");
    test1.Add(1, "world");
    test1.Add(2, "rld");
    test1.Add(3, "ell");
    test1.Add(4, "hell");
    assert(test1.Search("hello world!") == (set<int> { 0, 1, 2, 3, 4 }));
    assert(test1.Search("hEllo world!") == (set<int> { 1, 2 }));

    CSearch <string, bool (*)(const char&, const char&)> test2(upperCaseCompare);
    test2.Add(0, "hello");
    test2.Add(1, "world");
    test2.Add(2, "rld");
    test2.Add(3, "ell");
    test2.Add(4, "hell");
    assert(test2.Search("HeLlO WoRlD!") == (set<int> { 0, 1, 2, 3, 4 }));
    assert(test2.Search("hallo world!") == (set<int> { 1, 2 }));

    CSearch <string, CharComparator> test3(CharComparator(false));
    test3.Add(0, "hello");
    test3.Add(1, "world");
    test3.Add(2, "rld");
    test3.Add(3, "ell");
    test3.Add(4, "hell");
    assert(test3.Search("heLLo world!") == (set<int> { 0, 1, 2, 3, 4 }));
    assert(test3.Search("Well, templates are hell") == (set<int> { 3, 4 }));

    CSearch <vector<int> > test4;
    test4.Add(0, { 1, 6, 1, 6, 9, 12 });
    test4.Add(1, { 9, 12, 7 });
    assert(test4.Search(vector<int> { 1, 6, 1, 6, 1, 6, 9, 12, 8 }) == (set<int> { 0 }));
    assert(test4.Search(vector<int> { 1, 1, 6, 1, 6, 8, 12, 8 }) == (set<int> {}));

    CSearch <vector<string> > test5;
    test5.Add(0, { "Prague", "Bern", "Rome" });
    test5.Add(1, { "London", "Prague", "Bern" });
    test5.Add(2, { "London", "Rome" });
    assert(test5.Search(vector<string> { "Berlin", "London", "Prague", "Bern", "Rome", "Moscow" }) == (set<int> { 0, 1 }));

    CSearch <deque<int> > test6;
    test6.Add(0, { 1, 6, 1, 6, 9, 12 });
    test6.Add(1, { 9, 12, 7 });
    assert(test6.Search(deque<int> { 1, 6, 1, 6, 1, 6, 9, 12, 8 }) == (set<int> { 0 }));

    CSearch <forward_list<int> > test7;
    test7.Add(0, { 1, 6, 1, 6, 9, 12 });
    test7.Add(1, { 9, 12, 7 });
    assert(test7.Search(forward_list<int> { 1, 6, 1, 6, 1, 6, 9, 12, 8 }) == (set<int> { 0 }));

    CSearch <list<int> > test8;
    test8.Add(0, { 1, 6, 1, 6, 9, 12 });
    test8.Add(1, { 9, 12, 7 });
    assert(test8.Search(list<int> { 1, 6, 1, 6, 1, 6, 9, 12, 8 }) == (set<int> { 0 }));


    return 0;
}
#endif /* __PROGTEST__ */
#endif

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

template<typename T, typename C> 
class CFirsttemplate
{
public:
    CFirsttemplate(C c);
    ~CFirsttemplate();
    void addElement(const T& element);
    void sort();
    friend ostream& operator<< <>(ostream& os, const CFirsttemplate<T, C>& temp);

private:
    vector<T> m_VectorT;
    C m_Compare;
};

template<typename T, typename C>
CFirsttemplate<T, C>::CFirsttemplate(C c) :m_Compare(c)
{
   
}

template<typename T, typename C>
CFirsttemplate<T, C>::~CFirsttemplate()
{

} 

template<typename T, typename C>
void CFirsttemplate<T, C>::addElement(const T& element)
{
    this->m_VectorT.push_back(element);
}

bool CompareChar(const char& a, const char& b)
{
    return a > b;
}

class IntComparator
{
public:
    IntComparator() { ; }
    bool operator()(const int& a, const int& b) const { return a > b; }
private:
}; 

template<typename T, typename C>
void CFirsttemplate<T, C>::sort()
{
    // Use pointer to function as third argument
//    std::sort(this->m_VectorT.begin(), this->m_VectorT.end(), compareT<T>);
    // Use object of class Comparator. Note that class Comparator must have operator() 
    std::sort(this->m_VectorT.begin(), this->m_VectorT.end(), this->m_Compare);
    // Lyambda function
//    std::sort(this->m_VectorT.begin(), this->m_VectorT.end(), [](const T& a, const T& b) { return a > b; });
}

template<typename T, typename C>
ostream& operator<<(ostream& os, const CFirsttemplate<T, C>& temp)
{
    ostream_iterator<T> outit(os, ",");
    std::copy(temp.m_VectorT.begin(), temp.m_VectorT.end(), outit);
    return os;
}

int main(void)
{
//    CFirsttemplate<int, IntComparator> v10(IntComparator());
//    CSearch <string, CharComparator> test3(CharComparator(false));
//    v10.addElement(10);
//    v10.addElement(5);
//    v10.addElement(7);
//    v10.sort();
//    cout << v10 << endl;
    
    CFirsttemplate<string, bool (*) (const char&, const char&)> v2(CompareChar);
    v2.addElement("Petya");
    v2.addElement("John");
    v2.addElement("Vasya");
    v2.sort();
    cout << v2 << endl;


/*    CFirsttemplate<vector<int>> v3;
    v3.addElement({ 10, 20, 30 });
    v3.addElement({ 1, 2, 3, 4 });
    v3.sort();
 //   cout << v3 << endl;
 */
    return 0;
}

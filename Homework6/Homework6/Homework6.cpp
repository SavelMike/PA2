#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

// This is base class
class CCell
{
public:
    CCell() :m_Content() { ; }
    virtual void SetText(const char *text);
    virtual CCell & operator=(const CCell& orig);
    virtual void print(ostream& os, unsigned index, int width, int height) const { os << "I am CCell::print"; }
    const vector<string>& get_content() { return this->m_Content; }

protected: // Allows access to m_Content from derived class
    vector<string> m_Content;
};

void CCell::SetText(const char *text)
{
    assert(0 == 1);
}

CCell& CCell::operator=(const CCell& orig)
{
    assert(0 == 1);
    return *this;
}

class CText :public CCell
{
public:
    CText() : m_Align(ALIGN_LEFT) { ; }
    CText(const char* str, int);
    virtual void SetText(const char*);
    static const int ALIGN_LEFT = 1;
    static const int ALIGN_RIGHT = 2;
    virtual CCell& operator=(const CCell& orig);
    virtual void print(ostream& os, unsigned index, int width, int height) const;
private:
    int m_Align;
};

class CEmpty :public CCell
{
public:
    CEmpty() { ; }
    virtual CCell& operator=(const CCell& orig) { return *this; }
    virtual void SetText(const char*) { throw "CEmpty::SetText is not to be called"; }
    virtual void print(ostream& os, unsigned index, int width, int height) const;
private:
};

void CEmpty::print(ostream& os, unsigned index, int width, int height) const 
{
    os << setw(width) << ' ';
}

class CImage :public CCell
{
public:
    CImage();
    CImage& AddRow(const char* str);
    virtual CCell& operator=(const CCell& orig);
    virtual void SetText(const char*) { throw "CImage::SetText is not to be called"; }
    virtual void print(ostream& os, unsigned index, int width, int height) const;
private:
};

class CTable
{
public:    
    CTable(int rows, int columns);
    CTable(const CTable&);
    CTable& operator=(const CTable&);
    ~CTable();

    void SetCell(int row, int col, const CCell& cell);

    CCell & GetCell(int row, int col);
    friend ostream& operator <<(ostream& os, const CTable&);
private:
    CCell*** m_Table; // 2D Array of pointers to CCells
    int m_Rows;
    int m_Cols;
};

CTable::CTable(int rows, int columns) :m_Rows(rows), m_Cols(columns)
{
    this->m_Table = new CCell** [rows];
    for (int i = 0; i < rows; i++) {
        this->m_Table[i] = new CCell * [columns];
        for (int j = 0; j < columns; j++) {
            this->m_Table[i][j] = new CEmpty;
        }
    }
}

CTable::CTable(const CTable& table) :m_Rows(table.m_Rows), m_Cols(table.m_Cols)
{
    this->m_Table = new CCell** [table.m_Rows];
    for (int i = 0; i < table.m_Rows; i++) {
        this->m_Table[i] = new CCell * [table.m_Cols];
        for (int j = 0; j < table.m_Cols; j++) {
            this->m_Table[i][j] = new CEmpty;
            this->SetCell(i, j, *table.m_Table[i][j]);
        }
    }
}

CTable& CTable::operator=(const CTable& table)
{
    if (this == &table) {
        return *this;
    }

    for (int i = 0; i < this->m_Rows; i++) {
        for (int j = 0; j < this->m_Cols; j++) {
            delete this->m_Table[i][j];
        }
        delete[] this->m_Table[i];
    }
    delete[] this->m_Table;

    this->m_Rows = table.m_Rows;
    this->m_Cols = table.m_Cols;
    this->m_Table = new CCell * *[table.m_Rows];
    for (int i = 0; i < table.m_Rows; i++) {
        this->m_Table[i] = new CCell * [table.m_Cols];
        for (int j = 0; j < table.m_Cols; j++) {
            this->m_Table[i][j] = new CEmpty;
            this->SetCell(i, j, *table.m_Table[i][j]);
        }
    }

    return *this;
}

CTable::~CTable()
{
    for (int i = 0; i < this->m_Rows; i++) {
        for (int j = 0; j < this->m_Cols; j++) {
            delete this->m_Table[i][j];
        }
        delete[] this->m_Table[i];
    }
    delete[] this->m_Table;
}

CCell& CTable::GetCell(int row, int col)
{
    return *this->m_Table[row][col];
}

void CTable::SetCell(int row, int col, const CCell& cell)
{
    if (this->m_Table[row][col] != 0) {
        delete this->m_Table[row][col];
    }
    if (dynamic_cast<const CEmpty*> (&cell)) {
        // cell is CEmpty
        this->m_Table[row][col] = new CEmpty();
    } else if (dynamic_cast<const CText*> (&cell)) {
        // cell is CText
        this->m_Table[row][col] = new CText();
    } else if (dynamic_cast<const CImage*> (&cell)) {
        // cell is CImage
        this->m_Table[row][col] = new CImage();
    } else {
        throw "Unexpected object";
    }
    *this->m_Table[row][col] = cell;
}

/* this is used by std::max_element to find the longest string */
bool compare_strings_by_length(const string& a, const string& b)
{
    return a.length() < b.length();
}

// This is main function
ostream& operator <<(ostream& os, const CTable& table)
{
    // Calculates height and width of cells
    int* columnwidth = new int [table.m_Cols];
    for (int i = 0; i < table.m_Cols; i++) {
        columnwidth[i] = 0;
    }
    
    int* rowheight = new int[table.m_Rows];
    for (int i = 0; i < table.m_Rows; i++) {
        rowheight[i] = 0;
    }
    
    for (int i = 0; i < table.m_Rows; i++) {
        for (int j = 0; j < table.m_Cols; j++) {
            // Width and height of this cell
            if (table.m_Table[i][j] == NULL) {
                continue;
            }
            vector<string>::const_iterator it;
            it = max_element(table.m_Table[i][j]->get_content().begin(), 
                                table.m_Table[i][j]->get_content().end(), 
                                compare_strings_by_length);
            if (it == table.m_Table[i][j]->get_content().end()) {
                continue;
            }
            int w = it->size();
            if (w > columnwidth[j]) {
                columnwidth[j] = w;
            }
            int h = table.m_Table[i][j]->get_content().size();
            if (h > rowheight[i]) {
                rowheight[i] = h;
            }
        }
    }
    for (int i = 0; i < table.m_Rows; i++) {
        // Draw upper border of table
        for (int j = 0; j < table.m_Cols; j++) {
            os << '+' << string(columnwidth[j], '-');
        }
        os << '+' << endl;
        for (int j = 0; j < rowheight[i]; j++) {
            for (int k = 0; k < table.m_Cols; k++) {
                os << '|';
                table.m_Table[i][k]->print(os, j, columnwidth[k], rowheight[i]);
            }
            os << '|' << endl;
        }
    }
    for (int i = 0; i < table.m_Cols; i++) {
        os << '+' << string(columnwidth[i], '-');
    }
    os << '+' << endl;

    delete[] columnwidth;
    delete[] rowheight;
    return os;
}

CText::CText(const char* str, int align) :m_Align(align)
{
    string line;
    istringstream ss(str);
    while (getline(ss, line)) {
        m_Content.push_back(line);
    }
}

void CText::SetText(const char * str)
{
    // Delete previous content of m_Content
    this->m_Content.clear();
    
    string line;
    istringstream ss(str);
    while (getline(ss, line)) {
        this->m_Content.push_back(line);
    }
}

CCell& CText::operator=(const CCell& orig)
{
    const CText* p = dynamic_cast<const CText*> (&orig);
    this->m_Content = p->m_Content;
    this->m_Align = p->m_Align;

    return *this;
}

void CText::print(ostream& os, unsigned index, int width, int height) const
{
    if (this->m_Content.size() <= index) {
        os << setw(width) << ' ';
    }
    else {
        if (this->m_Align == ALIGN_LEFT) {
            os << left << setw(width) << this->m_Content[index];
        }
        else {
            os << right << setw(width) << this->m_Content[index];
        }
    }
}


CImage::CImage()
{
    
}

CImage& CImage::AddRow(const char* str)
{
    this->m_Content.push_back(str);
    return *this;
}

CCell& CImage::operator=(const CCell& orig)
{
    const CImage* p = dynamic_cast<const CImage*> (&orig);
    this->m_Content = p->m_Content;

    return *this;
}

void CImage::print(ostream& os, unsigned index, int width, int height) const 
{ 
    int vpad = height - this->m_Content.size();
    unsigned topvpad = vpad / 2;
    int botvpad = vpad - topvpad;
    if (topvpad > index) {
        os << setw(width) << ' ';
        return;
    }
    index -= topvpad;
    if (this->m_Content.size() <= index) {
        os << setw(width) << ' ';
    }
    else {
        int pad = width - this->m_Content[index].size();
        int lpad = pad / 2;
        int rpad = pad - lpad;
        os << string(lpad, ' ') << this->m_Content[index] << string(rpad, ' ');
    }
}

#ifndef __PROGTEST__
int main ( void )
{
  ostringstream oss;
  CTable t0 ( 3, 2 );
  t0 . SetCell ( 0, 0, CText ( "Hello,\n"
        "Hello Kitty", CText::ALIGN_LEFT ) );
  t0 . SetCell ( 1, 0, CText ( "Lorem ipsum dolor sit amet", CText::ALIGN_LEFT ) );
  t0 . SetCell ( 2, 0, CText ( "Bye,\n"
        "Hello Kitty", CText::ALIGN_RIGHT ) );
  t0 . SetCell ( 1, 1, CImage ()
        . AddRow ( "###                   " )
        . AddRow ( "#  #                  " )
        . AddRow ( "#  # # ##   ###    ###" )
        . AddRow ( "###  ##    #   #  #  #" )
        . AddRow ( "#    #     #   #  #  #" )
        . AddRow ( "#    #     #   #  #  #" )
        . AddRow ( "#    #      ###    ###" )
        . AddRow ( "                     #" )
        . AddRow ( "                   ## " )
        . AddRow ( "                      " )
        . AddRow ( " #    ###   ###   #   " )
        . AddRow ( "###  #   # #     ###  " )
        . AddRow ( " #   #####  ###   #   " )
        . AddRow ( " #   #         #  #   " )
        . AddRow ( "  ##  ###   ###    ## " ) );
  t0 . SetCell ( 2, 1, CEmpty () );
  oss . str ("");
  oss . clear ();
  oss << t0;
  assert ( oss . str () ==
        "+--------------------------+----------------------+\n"
        "|Hello,                    |                      |\n"
        "|Hello Kitty               |                      |\n"
        "+--------------------------+----------------------+\n"
        "|Lorem ipsum dolor sit amet|###                   |\n"
        "|                          |#  #                  |\n"
        "|                          |#  # # ##   ###    ###|\n"
        "|                          |###  ##    #   #  #  #|\n"
        "|                          |#    #     #   #  #  #|\n"
        "|                          |#    #     #   #  #  #|\n"
        "|                          |#    #      ###    ###|\n"
        "|                          |                     #|\n"
        "|                          |                   ## |\n"
        "|                          |                      |\n"
        "|                          | #    ###   ###   #   |\n"
        "|                          |###  #   # #     ###  |\n"
        "|                          | #   #####  ###   #   |\n"
        "|                          | #   #         #  #   |\n"
        "|                          |  ##  ###   ###    ## |\n"
        "+--------------------------+----------------------+\n"
        "|                      Bye,|                      |\n"
        "|               Hello Kitty|                      |\n"
        "+--------------------------+----------------------+\n" );
  t0 . SetCell ( 0, 1, t0 . GetCell ( 1, 1 ) );
  t0 . SetCell ( 2, 1, CImage ()
        . AddRow ( "*****   *      *  *      ******* ******  *" )
        . AddRow ( "*    *  *      *  *      *            *  *" )
        . AddRow ( "*    *  *      *  *      *           *   *" )
        . AddRow ( "*    *  *      *  *      *****      *    *" )
        . AddRow ( "****    *      *  *      *         *     *" )
        . AddRow ( "*  *    *      *  *      *        *       " )
        . AddRow ( "*   *   *      *  *      *       *       *" )
        . AddRow ( "*    *    *****   ****** ******* ******  *" ) );
  dynamic_cast<CText &> ( t0 . GetCell ( 1, 0 ) ) . SetText ( "Lorem ipsum dolor sit amet,\n"
        "consectetur adipiscing\n"
        "elit. Curabitur scelerisque\n"
        "lorem vitae lectus cursus,\n"
        "vitae porta ante placerat. Class aptent taciti\n"
        "sociosqu ad litora\n"
        "torquent per\n"
        "conubia nostra,\n"
        "per inceptos himenaeos.\n"
        "\n"
        "Donec tincidunt augue\n"
        "sit amet metus\n"
        "pretium volutpat.\n"
        "Donec faucibus,\n"
        "ante sit amet\n"
        "luctus posuere,\n"
        "mauris tellus" );
  oss . str ("");
  oss . clear ();
  oss << t0;
  assert ( oss . str () ==
        "+----------------------------------------------+------------------------------------------+\n"
        "|Hello,                                        |          ###                             |\n"
        "|Hello Kitty                                   |          #  #                            |\n"
        "|                                              |          #  # # ##   ###    ###          |\n"
        "|                                              |          ###  ##    #   #  #  #          |\n"
        "|                                              |          #    #     #   #  #  #          |\n"
        "|                                              |          #    #     #   #  #  #          |\n"
        "|                                              |          #    #      ###    ###          |\n"
        "|                                              |                               #          |\n"
        "|                                              |                             ##           |\n"
        "|                                              |                                          |\n"
        "|                                              |           #    ###   ###   #             |\n"
        "|                                              |          ###  #   # #     ###            |\n"
        "|                                              |           #   #####  ###   #             |\n"
        "|                                              |           #   #         #  #             |\n"
        "|                                              |            ##  ###   ###    ##           |\n"
        "+----------------------------------------------+------------------------------------------+\n"
        "|Lorem ipsum dolor sit amet,                   |                                          |\n"
        "|consectetur adipiscing                        |          ###                             |\n"
        "|elit. Curabitur scelerisque                   |          #  #                            |\n"
        "|lorem vitae lectus cursus,                    |          #  # # ##   ###    ###          |\n"
        "|vitae porta ante placerat. Class aptent taciti|          ###  ##    #   #  #  #          |\n"
        "|sociosqu ad litora                            |          #    #     #   #  #  #          |\n"
        "|torquent per                                  |          #    #     #   #  #  #          |\n"
        "|conubia nostra,                               |          #    #      ###    ###          |\n"
        "|per inceptos himenaeos.                       |                               #          |\n"
        "|                                              |                             ##           |\n"
        "|Donec tincidunt augue                         |                                          |\n"
        "|sit amet metus                                |           #    ###   ###   #             |\n"
        "|pretium volutpat.                             |          ###  #   # #     ###            |\n"
        "|Donec faucibus,                               |           #   #####  ###   #             |\n"
        "|ante sit amet                                 |           #   #         #  #             |\n"
        "|luctus posuere,                               |            ##  ###   ###    ##           |\n"
        "|mauris tellus                                 |                                          |\n"
        "+----------------------------------------------+------------------------------------------+\n"
        "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
        "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
        "|                                              |*    *  *      *  *      *           *   *|\n"
        "|                                              |*    *  *      *  *      *****      *    *|\n"
        "|                                              |****    *      *  *      *         *     *|\n"
        "|                                              |*  *    *      *  *      *        *       |\n"
        "|                                              |*   *   *      *  *      *       *       *|\n"
        "|                                              |*    *    *****   ****** ******* ******  *|\n"
        "+----------------------------------------------+------------------------------------------+\n" );
  CTable t1 ( t0 );
  t1 . SetCell ( 1, 0, CEmpty () );
  t1 . SetCell ( 1, 1, CEmpty () );
  oss . str ("");
  oss . clear ();
  oss << t0;
  assert ( oss . str () ==
        "+----------------------------------------------+------------------------------------------+\n"
        "|Hello,                                        |          ###                             |\n"
        "|Hello Kitty                                   |          #  #                            |\n"
        "|                                              |          #  # # ##   ###    ###          |\n"
        "|                                              |          ###  ##    #   #  #  #          |\n"
        "|                                              |          #    #     #   #  #  #          |\n"
        "|                                              |          #    #     #   #  #  #          |\n"
        "|                                              |          #    #      ###    ###          |\n"
        "|                                              |                               #          |\n"
        "|                                              |                             ##           |\n"
        "|                                              |                                          |\n"
        "|                                              |           #    ###   ###   #             |\n"
        "|                                              |          ###  #   # #     ###            |\n"
        "|                                              |           #   #####  ###   #             |\n"
        "|                                              |           #   #         #  #             |\n"
        "|                                              |            ##  ###   ###    ##           |\n"
        "+----------------------------------------------+------------------------------------------+\n"
        "|Lorem ipsum dolor sit amet,                   |                                          |\n"
        "|consectetur adipiscing                        |          ###                             |\n"
        "|elit. Curabitur scelerisque                   |          #  #                            |\n"
        "|lorem vitae lectus cursus,                    |          #  # # ##   ###    ###          |\n"
        "|vitae porta ante placerat. Class aptent taciti|          ###  ##    #   #  #  #          |\n"
        "|sociosqu ad litora                            |          #    #     #   #  #  #          |\n"
        "|torquent per                                  |          #    #     #   #  #  #          |\n"
        "|conubia nostra,                               |          #    #      ###    ###          |\n"
        "|per inceptos himenaeos.                       |                               #          |\n"
        "|                                              |                             ##           |\n"
        "|Donec tincidunt augue                         |                                          |\n"
        "|sit amet metus                                |           #    ###   ###   #             |\n"
        "|pretium volutpat.                             |          ###  #   # #     ###            |\n"
        "|Donec faucibus,                               |           #   #####  ###   #             |\n"
        "|ante sit amet                                 |           #   #         #  #             |\n"
        "|luctus posuere,                               |            ##  ###   ###    ##           |\n"
        "|mauris tellus                                 |                                          |\n"
        "+----------------------------------------------+------------------------------------------+\n"
        "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
        "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
        "|                                              |*    *  *      *  *      *           *   *|\n"
        "|                                              |*    *  *      *  *      *****      *    *|\n"
        "|                                              |****    *      *  *      *         *     *|\n"
        "|                                              |*  *    *      *  *      *        *       |\n"
        "|                                              |*   *   *      *  *      *       *       *|\n"
        "|                                              |*    *    *****   ****** ******* ******  *|\n"
        "+----------------------------------------------+------------------------------------------+\n" );
  oss . str ("");
  oss . clear ();
  oss << t1;
  assert ( oss . str () ==
        "+-----------+------------------------------------------+\n"
        "|Hello,     |          ###                             |\n"
        "|Hello Kitty|          #  #                            |\n"
        "|           |          #  # # ##   ###    ###          |\n"
        "|           |          ###  ##    #   #  #  #          |\n"
        "|           |          #    #     #   #  #  #          |\n"
        "|           |          #    #     #   #  #  #          |\n"
        "|           |          #    #      ###    ###          |\n"
        "|           |                               #          |\n"
        "|           |                             ##           |\n"
        "|           |                                          |\n"
        "|           |           #    ###   ###   #             |\n"
        "|           |          ###  #   # #     ###            |\n"
        "|           |           #   #####  ###   #             |\n"
        "|           |           #   #         #  #             |\n"
        "|           |            ##  ###   ###    ##           |\n"
        "+-----------+------------------------------------------+\n"
        "+-----------+------------------------------------------+\n"
        "|       Bye,|*****   *      *  *      ******* ******  *|\n"
        "|Hello Kitty|*    *  *      *  *      *            *  *|\n"
        "|           |*    *  *      *  *      *           *   *|\n"
        "|           |*    *  *      *  *      *****      *    *|\n"
        "|           |****    *      *  *      *         *     *|\n"
        "|           |*  *    *      *  *      *        *       |\n"
        "|           |*   *   *      *  *      *       *       *|\n"
        "|           |*    *    *****   ****** ******* ******  *|\n"
        "+-----------+------------------------------------------+\n" );
  t1 = t0;
  t1 . SetCell ( 0, 0, CEmpty () );
  t1 . SetCell ( 1, 1, CImage ()
        . AddRow ( "  ********                    " )
        . AddRow ( " **********                   " )
        . AddRow ( "**        **                  " )
        . AddRow ( "**             **        **   " )
        . AddRow ( "**             **        **   " )
        . AddRow ( "***         ********  ********" )
        . AddRow ( "****        ********  ********" )
        . AddRow ( "****           **        **   " )
        . AddRow ( "****           **        **   " )
        . AddRow ( "****      **                  " )
        . AddRow ( " **********                   " )
        . AddRow ( "  ********                    " ) );
  oss . str ("");
  oss . clear ();
  oss << t0;
  assert ( oss . str () ==
        "+----------------------------------------------+------------------------------------------+\n"
        "|Hello,                                        |          ###                             |\n"
        "|Hello Kitty                                   |          #  #                            |\n"
        "|                                              |          #  # # ##   ###    ###          |\n"
        "|                                              |          ###  ##    #   #  #  #          |\n"
        "|                                              |          #    #     #   #  #  #          |\n"
        "|                                              |          #    #     #   #  #  #          |\n"
        "|                                              |          #    #      ###    ###          |\n"
        "|                                              |                               #          |\n"
        "|                                              |                             ##           |\n"
        "|                                              |                                          |\n"
        "|                                              |           #    ###   ###   #             |\n"
        "|                                              |          ###  #   # #     ###            |\n"
        "|                                              |           #   #####  ###   #             |\n"
        "|                                              |           #   #         #  #             |\n"
        "|                                              |            ##  ###   ###    ##           |\n"
        "+----------------------------------------------+------------------------------------------+\n"
        "|Lorem ipsum dolor sit amet,                   |                                          |\n"
        "|consectetur adipiscing                        |          ###                             |\n"
        "|elit. Curabitur scelerisque                   |          #  #                            |\n"
        "|lorem vitae lectus cursus,                    |          #  # # ##   ###    ###          |\n"
        "|vitae porta ante placerat. Class aptent taciti|          ###  ##    #   #  #  #          |\n"
        "|sociosqu ad litora                            |          #    #     #   #  #  #          |\n"
        "|torquent per                                  |          #    #     #   #  #  #          |\n"
        "|conubia nostra,                               |          #    #      ###    ###          |\n"
        "|per inceptos himenaeos.                       |                               #          |\n"
        "|                                              |                             ##           |\n"
        "|Donec tincidunt augue                         |                                          |\n"
        "|sit amet metus                                |           #    ###   ###   #             |\n"
        "|pretium volutpat.                             |          ###  #   # #     ###            |\n"
        "|Donec faucibus,                               |           #   #####  ###   #             |\n"
        "|ante sit amet                                 |           #   #         #  #             |\n"
        "|luctus posuere,                               |            ##  ###   ###    ##           |\n"
        "|mauris tellus                                 |                                          |\n"
        "+----------------------------------------------+------------------------------------------+\n"
        "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
        "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
        "|                                              |*    *  *      *  *      *           *   *|\n"
        "|                                              |*    *  *      *  *      *****      *    *|\n"
        "|                                              |****    *      *  *      *         *     *|\n"
        "|                                              |*  *    *      *  *      *        *       |\n"
        "|                                              |*   *   *      *  *      *       *       *|\n"
        "|                                              |*    *    *****   ****** ******* ******  *|\n"
        "+----------------------------------------------+------------------------------------------+\n" );
  oss . str ("");
  oss . clear ();
  oss << t1;
  assert ( oss . str () ==
        "+----------------------------------------------+------------------------------------------+\n"
        "|                                              |          ###                             |\n"
        "|                                              |          #  #                            |\n"
        "|                                              |          #  # # ##   ###    ###          |\n"
        "|                                              |          ###  ##    #   #  #  #          |\n"
        "|                                              |          #    #     #   #  #  #          |\n"
        "|                                              |          #    #     #   #  #  #          |\n"
        "|                                              |          #    #      ###    ###          |\n"
        "|                                              |                               #          |\n"
        "|                                              |                             ##           |\n"
        "|                                              |                                          |\n"
        "|                                              |           #    ###   ###   #             |\n"
        "|                                              |          ###  #   # #     ###            |\n"
        "|                                              |           #   #####  ###   #             |\n"
        "|                                              |           #   #         #  #             |\n"
        "|                                              |            ##  ###   ###    ##           |\n"
        "+----------------------------------------------+------------------------------------------+\n"
        "|Lorem ipsum dolor sit amet,                   |                                          |\n"
        "|consectetur adipiscing                        |                                          |\n"
        "|elit. Curabitur scelerisque                   |        ********                          |\n"
        "|lorem vitae lectus cursus,                    |       **********                         |\n"
        "|vitae porta ante placerat. Class aptent taciti|      **        **                        |\n"
        "|sociosqu ad litora                            |      **             **        **         |\n"
        "|torquent per                                  |      **             **        **         |\n"
        "|conubia nostra,                               |      ***         ********  ********      |\n"
        "|per inceptos himenaeos.                       |      ****        ********  ********      |\n"
        "|                                              |      ****           **        **         |\n"
        "|Donec tincidunt augue                         |      ****           **        **         |\n"
        "|sit amet metus                                |      ****      **                        |\n"
        "|pretium volutpat.                             |       **********                         |\n"
        "|Donec faucibus,                               |        ********                          |\n"
        "|ante sit amet                                 |                                          |\n"
        "|luctus posuere,                               |                                          |\n"
        "|mauris tellus                                 |                                          |\n"
        "+----------------------------------------------+------------------------------------------+\n"
        "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
        "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
        "|                                              |*    *  *      *  *      *           *   *|\n"
        "|                                              |*    *  *      *  *      *****      *    *|\n"
        "|                                              |****    *      *  *      *         *     *|\n"
        "|                                              |*  *    *      *  *      *        *       |\n"
        "|                                              |*   *   *      *  *      *       *       *|\n"
        "|                                              |*    *    *****   ****** ******* ******  *|\n"
        "+----------------------------------------------+------------------------------------------+\n" );

  return 0;
}
#endif /* __PROGTEST__ */

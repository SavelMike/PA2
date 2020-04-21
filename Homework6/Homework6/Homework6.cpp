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

class CCell
{
public:
    CCell() { ; }
private:

};

class CText;
class CEmpty;
class CImage;

class CTable
{
public:    
    CTable(int rows, int colums);
    void SetCell(int row, int col, const CCell& cell);
  //  void SetCell(int row, int col, const CEmpty& cempty);
  //  void SetCell(int row, int col, const CImage& cimage);
    CCell & GetCell(int row, int col);
    friend ostream& operator <<(ostream& os, const CTable&);
};

class CText:public CCell
{
public:
    CText(const char* str, int);
    static const int ALIGN_LEFT = 1;
    static const int ALIGN_RIGHT = 2;
private:
};

class CEmpty:public CCell
{
public: 
    CEmpty() { ; }
private:
};

class CImage:public CCell
{
public:
    CImage() { ; }
    CImage & AddRow(const char* str);
private:
};

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

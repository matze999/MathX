#if 1

#include <gtest/gtest.h>
#include "BasicsTest.h"
#include "RuleTest.h"
#include "ClassParserTest.h"
#include "GrammarTest.h"

#include <util.h>
#include <initializer_list>
#include "test_base.h"
#include "test_simple.h"
#include "test_charclass.h"
////#include "test_grammer.h"
//#include "test_wc.h"
//#include "test_backref.h"
//#include "test_expression.h"
//#include "test_operator.h"
//

template <class T>
struct nargs;

template <class T, class R, class ...Args>
struct nargs < R (T::*) (Args...) >
{
   static const int value = sizeof... (Args);
};


template <class R, class ...Args>
struct nargs < R (Args...) >
{
   static const int value = sizeof... (Args);
};


int func (int)  { return 4; }

class A
{
public:
   template <class S>
   bool parse (S)  { return true; }

   template <class S>
   bool parse (S, int)  { return true; }
};

struct  Schadow
{
   int parse (unsigned);
};


using namespace std;


struct element
{
   int value;


   //element (initializer_list<int> list)

   bool operator() (int lb, int ub)
   {
      return true;
   }

   bool operator%= (initializer_list<int> list)
   {
      return true;
   }

   bool operator%= (int list)
   {
      return true;
   }
};


element& operator>> (int val, element& el)
{
   el.value = val;
   return el;
}




int main (int argc, char** argv)
{
   //TRC_LEVEL = TL_DEBUG;
   testing::InitGoogleTest (&argc, argv);

   RUN_ALL_TESTS (); 


      TEST_CASE (test_Simple);
      TEST_CASE (test_CharClass);
      //TEST_CASE (test_Backref);
      //TEST_CASE (testWC);
      //TEST_CASE (test_Switch);
      //TEST_CASE (test_Switch2);
      ////TEST_CASE (test_Map);
      //TEST_CASE (test_Option);
      //TEST_CASE (test_Operator);
      //TEST_CASE (test_Calculator2);
      

      //TEST_CASE (testGrammer);

      //test_Functor();
      //testCalculator();

   getchar ();
   return 0;
}



#else


#include <iostream>
//#include <iosff>
#include <meta/ilist.h>
#include <meta/typetraits.h>
#include <meta/has_function_call.h>
#include <meta/has_member.h>

#include <list>
#include <test/test_rvalue.h>

using namespace std;





void overloaded( int const &arg ) { std::cout << "by lvalue\n"; }
void overloaded( int && arg ) { std::cout << "by rvalue\n"; }

template< typename t >
/* "t &&" with "t" being template param is special, and  adjusts "t" to be
   (for example) "int &" or non-ref "int" so std::forward knows what to do. */
void forwarding( t && arg ) {
    std::cout << "via std::forward: ";
    overloaded( std::forward< t >( arg ) );
    std::cout << "via std::move: ";
    overloaded( std::move( arg ) ); // conceptually this would invalidate arg
    std::cout << "by simple passing: ";
    overloaded( arg );
}




#include <ComboList.h>
#include <utility>
#include <algorithm>
#include "VectorUtils.h"

using namespace mp;
using namespace mgo;





struct MySelection: public SelectionList <string>
{
   SelectionValue  red, green, blue;

   MySelection ():
      red (newSelectionValue ("red")),
      green (newSelectionValue ("green")),
      blue (newSelectionValue ("blue"))  
   {}
};


struct MyCombo: public SparseVector < string >
{
   SelectionValue  one, two, three;

   MyCombo ():
      one (newSelectionValue ("one")),
      two (newSelectionValue ("two")),
      three (newSelectionValue ("three"))  {}
};


using MyCombo2 = SparseVector < string, MySelection > ;



int main ()
{
   MySelection  selection;

   cout << "Selection: " << writeAsList (selection) << endl;


   SparseVector <string> list;

   auto red = list.newSelectionValue ("red"),
      blue = list.newSelectionValue ("blue"),
      green = list.newSelectionValue ("green");

   list.push_back (red);
   list.push_back (green);
   list.push_back (blue);
   list.push_back (blue);
   list.push_back (red);

   cout << "combo list: " << writeAsList (list) << endl;

   cout << "test: " << red << endl;


   MyCombo  combo;
   combo.push_back (combo.one);
   combo.push_back (combo.three);
   combo.push_back (combo.two);

   cout << "Combo: " << writeAsList (combo) << endl;
   cout << "SelectionList: " << writeAsList (combo.SelectionList ()) << endl;


   MyCombo2  combo2;
   combo2.push_back (combo2.SelectionList().blue);
   combo2.push_back (combo2.SelectionList ().red);
   combo2.push_back (combo2.SelectionList ().green);

   cout << "Combo: " << writeAsList (combo2) << endl;
   cout << "SelectionList: " << writeAsList (combo2.SelectionList ()) << endl;



   getchar ();
}


#endif


#include <gtest/gtest.h>


#include <iostream>
#include <util.h>
#include <meta/typetraits.h>
#include <meta/if.h>
#include <StrUtils.h>
//#include <boost/variant.hpp>
//#include <parser.h>
//#include "Number.h"
//#include "Operator.h"
//#include <test/test_rvalue.h>
using namespace std;
using namespace mp;





struct AA
{
   template <class T>
   T  get ()
   {
      cout << "get<T>" << endl;
      return T ();
   }


   template <>
   int get<int> ()
   {
      cout << "get<int>" << endl;
      return 4.22;
   }

   template <>
   double get<double> ()
   {
      cout << "get<double>" << endl;
      return 2.11;
   }

};


struct BB: public AA
{};



void func ()
{
   cout << "func start" << endl;
   scope_exit
   {
      cout << "scope end" << endl;
   };

   scope
   {
      cout << "scope start" << endl;
      scope_exit
      {
         cout << "scope end inside scope" << endl;
      };

      cout << "scope center" << endl;
      //if (true)  break;
      cout << "scope end" << endl;
   };

   cout << "func end" << endl;
}



template <class ...Args>
struct VarClass
{

};



TEST (first_test, test_case1)
{
   ASSERT_EQ (1, 1);
}


int main (int argc, char** argv)
{
   testing::InitGoogleTest (&argc, argv);

   RUN_ALL_TESTS (); 
 
 
   if (false)
   {
    VarClass<int, double>   vc;

    int val = 4;
    AA a;
    BB b;

    auto lambda = []() { cout << "lambda condition" << endl; return true;  };

    if (lambda ())  cout << "lambda output" << endl;
    func ();





    cout << a.get<double> () << endl;
    cout << a.get<int> () << endl;
    cout << a.get<string> () << endl;
   }


   getchar ();
   return 0;
}




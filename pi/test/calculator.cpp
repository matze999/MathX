
#include <fn.h>
//#include <stack.h>
#include "../parser.h"

using namespace pi;
using namespace mgo::fn;

using mgo::Stack;


struct  StackAction: public Stack<int>
{
   void  operator() (int val)
   {
      push (val);
   }

   template <class Func>
   void operator() (Func func)
   {
      assert (size() > 1)
      int rhs = top();
      pop();
      int lhs = top();

      top() = func (lhs, rhs);
   }
};


template <class F>
applyF<F> apply (F func)
{
   return applyF <F> (func);
}



int  calculate (const char *input)
{
   StackAction stack;

   Rule <action<StackAction>>  sum (stack), product(stack), term(stack);

   term =  int_;//  |  ("(" >> sum >> ")");

   product =  term  >>  *(("*" >> term >> apply (multiply()))  |
                          ("/" >> term >> apply (divide())) );

   sum =  product  &  *(("+" >> product >> apply (plus()))  |
                        ("-" >> product >> apply (minus()))  );

    



   Scanner scanner(input);
   if (sum.parse (scanner))
   {
      assert0 (stack.size() == 1);
      return stack.top();
   }
   else throw  scanner.getErrorInfo();
}
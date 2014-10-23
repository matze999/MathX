
#include <fn.h>
#include <stack.h>
#include <parser.h>

using namespace pi;



struct  CalcAttribute:  public vector_attribute_tag
{
   CalcAttribute (mgo::Stack<int>& data): stack (data)  {}

   void  push_back (int val)
   {
      TRC_DEBUG (calc) << "Stack: push: " << val << mgo::endl;
      stack->push (val);
   }

   template <class Func>
   void push_back (Func func)
   {
      assert0 (stack->size() > 1);

      int rhs = stack->top();
      stack->pop();
      int lhs = stack->top();

      stack->top() = func (lhs, rhs);

      TRC_DEBUG (calc) << "Stack: new top: " << stack->top() 
                       << "  size: " << stack->size() << mgo::endl;
   }

private:
   mgo::Reference <mgo::Stack<int>>  stack;
};




class Calculator: public Grammer <CalcAttribute, WhiteSpace>
{
public:
   Calculator (): StartRule (sum),
      sum (stack), product(stack), term (stack)
   {
      using namespace mgo::fn;
      sum =  product  >>  *(("+" >> product >> var (plus()))  |
                           ("-" >> product >> var (minus()))  );


      product =  term  >>  *(("*" >> term >> var (multiply()))  |
                             ("/" >> term >> var (divide())) );

      term =  int_  |  ("(" >> sum >> ")");
   }

   int Result ()
   {
      if (stack.size() == 1)
         cout << "Expression incompleate" << endl;
      return  stack.top ();
   }

private:
   mgo::Stack<int>  stack;
   Rule <CalcAttribute>  sum, product, term;
};


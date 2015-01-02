#ifndef __MATHX_OPERATOR_H
#define __MATHX_OPERATOR_H

#include <functional>
#include "Function.h"
#include <numreader.h>


namespace mathX {

struct BinaryOperator: public BinaryFunction
{
   template <class F>
   BinaryOperator (const char* name, const F& fun):
      BinaryFunction (Priority::Operator (*name), name, fun) {}

   void serialize (std::ostream& out) const
   {
      arg1->serializeWithBrackets (out, arg1->priority > priority);
      out << name;
      arg2->serializeWithBrackets (out, arg2->priority > priority);
   }
};



#define  MATHX_MAKE_OPERATOR(op, func)  \
inline Expression  operator op (const Expression& lhs, const Expression& rhs) {\
auto bfun = new BinaryOperator (#op, func()); \
bfun->setArgs(lhs.get(), rhs.get()); \
return  Expression(lhs.getAllocator(), bfun); }



MATHX_MAKE_OPERATOR (+, std::plus<double>)
MATHX_MAKE_OPERATOR (-, std::minus<double>)

MATHX_MAKE_OPERATOR (*, std::multiplies<double>)
MATHX_MAKE_OPERATOR (/ , std::divides<double>)


double  power_fun (double basis, double exp)
{
   return  std::pow (basis, exp);
}


Expression  pow (const Expression& basis, const Expression& exp)
{
   auto bfun = new BinaryOperator ("pow", power_fun);
   bfun->setArgs (basis.get (), exp.get ());
   return  Expression (basis.getAllocator (), bfun);
}



double isEqual (double lval, double rval)
{
   if (isnan (lval) || isnan (rval))  return nan ("");
   return mgo::isEqual (lval, rval);
}



Expression  eq (const Expression& lhs, const Expression& rhs)
{
   auto bfun = new BinaryOperator ("=", isEqual);
   bfun->setArgs (lhs.get (), rhs.get ());
   return  Expression (rhs.getAllocator (), bfun);
}




#undef MATHX_MAKE_OPERATOR


} // namespace mathX
#endif 


#ifndef __MATHX_OPERATOR_H
#define __MATHX_OPERATOR_H

#include <functional>
#include "Function.h"


namespace mathX {



inline
Expression  operator+ (const Expression& lhs, const Expression& rhs)
{
   auto func = new BinaryFunction ("+", std::plus<double> ());
   func->setArgs (lhs.get(), rhs.get());

   return  Expression(lhs.getAllocator(), func);
}

inline
Expression  operator- (const Expression& lhs, const Expression& rhs)
{
   auto func = new BinaryFunction ("-", std::minus<double> ());
   func->setArgs (lhs.get(), rhs.get());

   return  Expression(lhs.getAllocator(), func);
}


} // namespace mathX
#endif 


#ifndef __MATHX_OPERATOR_H
#define __MATHX_OPERATOR_H

#include <functional>
#include "Function.h"


namespace mathX {



#define  MATHX_MAKE_OPERATOR(op, func)  \
   inline Expression  operator op (const Expression& lhs, const Expression& rhs) {\
      auto bfun = new BinaryFunction(#op, func()); \
      bfun->setArgs(lhs.get(), rhs.get()); \
      return  Expression(lhs.getAllocator(), bfun); }



MATHX_MAKE_OPERATOR (+, std::plus<double>)
MATHX_MAKE_OPERATOR (-, std::minus<double>)

MATHX_MAKE_OPERATOR (*, std::multiplies<double>)
MATHX_MAKE_OPERATOR (/ , std::divides<double>)



#undef MATHX_MAKE_OPERATOR


} // namespace mathX
#endif 


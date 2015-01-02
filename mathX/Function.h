#ifndef __MATHX_FUNCTION_H
#define __MATHX_FUNCTION_H

#include <functional>
#include "MathExpression.h"


namespace mathX {



struct BinaryFunction: public make_expression <BinaryFunction>
{
   template <class F>
   BinaryFunction (const char* name, const F& fun): 
      Super (Priority::BINARY_FUNCTION),
      name (name),
      func (fun) {}


   BinaryFunction&  setArgs (const BaseExpression& first, const BaseExpression& second)
   {
      arg1 = &first;
      arg2 = &second;
      return *this;
   }


   double eval () const
   {
      return func (arg1->eval(), arg2->eval());
   }


   void serialize(std::ostream& out) const override
   {
      out << name << " (";
      arg1->serialize (out);
      out << ", ";
      arg2->serialize (out);
      out << ")";
   }

protected:
   template <class F>
   BinaryFunction(Priority::Level  priority, const char* name, const F& fun):
      Super (priority),
      name(name),
      func(fun) {}


   const char* name;
   std::function <double(double, double)>  func;

   const BaseExpression *arg1 = nullptr, *arg2 = nullptr;
};




} // namespace mathX
#endif 


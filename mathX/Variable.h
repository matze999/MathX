#ifndef __MATHX_VARIABLE_H
#define __MATHX_VARIABLE_H

#include "MathExpression.h"


namespace mathX {



struct Variable: public make_expression <Variable>
{
   explicit Variable (const std::string&& name = "var"): 
      Super(Priority::VARIABLE), name(name)  {}


   double eval () const
   {
      return nan("");
   }


   void serialize(std::ostream& out) const override
   {
      out << name;
   }


protected:
   std::string name;
};


BaseExpression*  makeExpression (std::string name)
{
   return  new Variable (std::move(name));
}


} // namespace mathX
#endif 


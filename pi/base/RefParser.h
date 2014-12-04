#ifndef _REF_PARSER_
#define _REF_PARSER_

#include <ErrorInfo.h>
#include "Base.h"
//#include "Attribute.h"
#include <StrUtils.h>

namespace pi { 

namespace _ {


template <class T, class Parser>
struct reference_p: public UnaryExpression <Parser, PID::EXPRESSION>
{
   //typedef typename mp::make_attribute<T, Assoc::Reference>::type   Attribute;
   typedef T&   Attribute;

   reference_p (T& value, const Parser &parser):
      attribute(value), UnaryExpression <Parser, PID::EXPRESSION> (parser)  {}


   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      BACK_TRACE;
      if (child.parse (scanner, attribute))  return true;

      return false;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      child.toString (out, level);
      out << '(' << getTypeId(T) << ')';
   }

private:
    mutable Attribute  attribute;
};


} // namespace _




} // namespace pi 
#endif 


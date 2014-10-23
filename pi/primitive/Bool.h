#ifndef _PARSER_BOOL_H_
#define _PARSER_BOOL_H_

#include "Literal.h"
#include <Lambda.h>

namespace pi { 
namespace _ {


struct nil_p
{
   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collector) const
   {
      return  MatchFailure (scanner);
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      out << "nil";
   }
};
    
       

struct eps_p
{
   enum { rank = PID::LITERAL };

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collector) const
   {
       return  true;
   }
       
   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      out << "eps";
   }
};


struct bool_p
{
//   typedef  bool  value_type;
   enum { rank = PID::LITERAL };

   template <class Scanner, class Attribute>
   bool parse (Scanner &scanner, Attribute &attribute) const
   {
      BACK_TRACE;
      if (lit_p ("true").parse (scanner))
         mp::assign (attribute, true);
      else 
		if (lit_p ("false").parse (scanner))
         mp::assign (attribute, false);
      else
         return false;

      return true;
   }
       
   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      out << "bool";
   }
};

} // namespace _



const BaseParser <_::eps_p>  eps;
const BaseParser <_::nil_p>  nil;

const BaseParser <_::bool_p>  bool_;



} // namespace pi 
#endif 


#ifndef _FUNCTOR_PARSER_
#define _FUNCTOR_PARSER_

#include <ErrorInfo.h>
#include "Base.h"
#include "Attribute.h"

namespace pi { 

namespace _ {


template <class Fun>
struct inset_p
{
   enum { rank = PID::EXPRESSION };

   inset_p (Fun& fun): function(fun) {}


   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      BACK_TRACE;
      fun (collector); 
      return true;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      out << "inset()";
   }

private:
    Fun  function;
};

} // namespace _


template <class Fun>
BaseParser <_::inset_p<Fun>>  inset (Fun& fun)
{
   BaseParser <_::inset_p<Fun>> (fun);
}

} // namespace pi 
#endif 


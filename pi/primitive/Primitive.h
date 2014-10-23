#ifndef _PARSER_PRIMITIVE_H_
#define _PARSER_PRIMITIVE_H_

#include "../base/parser.h"

namespace pi { 
namespace _ {


template <class P>
struct prim_p
{
   prim_p (const P& parser): child (parser)  {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collector) const
   {
         
      if (child.parse (scanner, collector))  return true;

      return  MatchFailure (scanner, *this);
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      child.toString (out, level);
   }


private:
   ref_to <P>::type child;
};
    
 
} // namespace _


struct PrimHelper
{
   template <class P>
   BaseParser <_::prim_p<P>>  operator[] (const P& parser) const
   {
      return BaseParser <_::prim_p<P>> (parser);
   }
};

const PrimHelper  prim;


} // namespace pi 
#endif 


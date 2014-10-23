#ifndef  _PARSER_ALTERNATIVE_H_
#define  _PARSER_ALTERNATIVE_H_

#include "MakeParser.h"


namespace pi { 
namespace _ {


template <class P, class Q, class = void>
struct or_p: public BinaryExpression <P, Q, PID::OR>
{
   or_p (const P &lhs, const Q &rhs): Super (lhs, rhs)  {}
      
   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      if (left.parse (scanner, collect))  return true;
      if (right.parse (scanner, collect))
      {
         scanner.getErrorInfo ().clearRecord ();
         return true;
      }
      return false;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      writeOperator (out, " | ", level);
   }
};

} // namespace _


template <class P, class Q>
typename mp::make_parser <_::or_p, P, Q>::type 
operator| (const P &lhs, const Q &rhs)
{
   return  mp::make_parser <_::or_p, P, Q>::execute (lhs, rhs);
}


} // namespace pi
#endif
   
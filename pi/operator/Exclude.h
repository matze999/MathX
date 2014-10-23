#ifndef  _PARSER_EXCLUDE_H_
#define  _PARSER_EXCLUDE_H_

#include "MakeParser.h"

namespace pi { 
namespace _ {


template <class P, class Q, class = void>
struct exclude_p: public BinaryExpression <P, Q, PID::MINUS>
{
   //typedef  typename P::value_type  value_type;


   exclude_p (const P &lhs, const Q &rhs): Super (lhs, rhs)  {}


   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const 
   {
      BACK_TRACE;
      Scanner::Iterator  spos = scanner.begin(); 
      if (!right.parse (scanner, fn::no_action())  &&
           left.parse (scanner, collect))
         return true;

      scanner.begin (spos);
      return false;
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      writeOperator (out, " / ", level);
   }
};


} // namespace _


template <class P, class Q>
typename  mp::make_parser <_::exclude_p, P, Q>::type
operator/ (const P &lhs, const Q &rhs)
{
   return  mp::make_parser <_::exclude_p, P, Q>::execute (lhs, rhs);
}

} // namespace pi
#endif

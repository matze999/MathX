#ifndef  _PARSER_SEQUENCE_H_
#define  _PARSER_SEQUENCE_H_

#include "MakeParser.h"

namespace pi { 
namespace _ {



template <class P, class Q, class = void>
struct and_p: public BinaryExpression <P, Q, PID::AND>
{
public:
   and_p (const P &lhs, const Q &rhs): Super (lhs, rhs)  {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const 
   {
      Scanner::Iterator  spos = scanner.begin();
      if (!left.parse (scanner, collect))  
         return  MatchFailure (scanner, left);

      if (right.parse (scanner, collect))  return true;

      MatchFailure (scanner, right);
      scanner.begin (spos);
      return false;
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      writeOperator (out, "  ", level);
   }
};



} // namespace _


template <class P, class Q>
typename  mp::make_parser <_::and_p, P, Q>::type
operator>> (const P& lhs, const Q& rhs)
{
   return  mp::make_parser <_::and_p, P, Q>::execute (lhs, rhs);
}


template <class P, class Q>
typename  mp::make_parser <_::and_p, P, Q>::type
operator& (const P& lhs, const Q& rhs)
{
   return  mp::make_parser <_::and_p, P, Q>::execute (lhs, rhs);
}


} // namespace pi
#endif

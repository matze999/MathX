#ifndef  _PARSER_ANDOR__
#define  _PARSER_ANDOR__


#include "../base/Parser.h"



namespace pi { 
namespace _ {


template <class P, class Q, class = void>
struct and_or_p: public BinaryExpression <P, Q, PID::XOR>
{
   and_or_p (const P& lhs, const Q& rhs): Super (lhs, rhs)  {}

   template <class Scanner, class Attribute>
   bool  parse (Scanner &scanner, Attribute& attribute) const
   {
      if (left.parse (scanner, attribute))  
         right.parse (scanner, attribute);
      else
      if (!right.parse (scanner, attribute))
         return false;

      return true;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      writeOperator (out, " ^ ", level);
   }
};


} // namespace _



template <class P, class Q>
typename mp::make_parser <_::and_or_p, P, Q>::type
operator^ (const P &lhs, const Q &rhs)
{
   return  mp::make_parser <_::and_or_p, P, Q>::execute (lhs, rhs);
}


} // namespace pi
#endif

#ifndef  _PARSER_OPTION__
#define  _PARSER_OPTION__


#include "../base/Parser.h"



namespace pi { 
namespace _ {


template <class P, class Q, class = void>
struct option_p: public BinaryExpression <P, Q, PID::COMMA>
{
   option_p (const P& lhs, const Q& rhs): Super (lhs, rhs)  {}

   template <class Scanner, class Attribute>
   bool  parse (Scanner &scanner, Attribute& attribute) const
   {
      if (left.parse (scanner, attribute))  
         right.parse (scanner, attribute);
      else
      if (right.parse (scanner, attribute))
         left.parse (scanner, attribute);
      else
         return false;

      return true;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      writeOperator (out, ", ", level);
   }
};


} // namespace _



template <class P, class Q>
typename mp::make_parser <_::option_p, P, Q>::type
operator, (const P &lhs, const Q &rhs)
{
   return  mp::make_parser <_::option_p, P, Q>::execute (lhs, rhs);
}


} // namespace pi
#endif

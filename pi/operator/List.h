#ifndef  _PARSER_LIST_H_
#define  _PARSER_LIST_H_

#include "../base/Parser.h"

namespace pi {

namespace _{



// list parser  - empty list ist allowed

template <class P, class Q, class = void>
struct list_p: public BinaryExpression <P, Q, PID::LIST>
{
   list_p (const P &lhs, const Q &rhs): Super (lhs, rhs)  {}
      
   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      BACK_TRACE;

      if (!left.parse (scanner, collect))  return true;
      while (right.parse (scanner, collect))
      {
         if (!left.parse (scanner, collect))  
            return false;
      }
      return true;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      writeOperator (out, " % ", level);
   }
};

} // namespace _   


template <class P, class Q>
typename mp::make_parser <_::list_p, P, Q>::type
operator% (const P &lhs, const Q &rhs)
{
   return  mp::make_parser <_::list_p, P, Q>::execute (lhs, rhs);
}


} // namespace pi
#endif
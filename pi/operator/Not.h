#ifndef  _PARSER_NOT_H_
#define  _PARSER_NOT_H_

#include "../base/Parser.h"

namespace pi {

namespace _{


template <class P>
struct  not_p: public UnaryExpression <P, PID::NOT>
{
   not_p (const P &lhs): Super (lhs)  {}
      
   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      BACK_TRACE;
      auto  pos = scanner.begin ();
      bool rc = child.parse (scanner, fn::no_action());
      scanner.begin (pos);
      return !rc;

   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      writeOperator (out, "!", level);
   }
};

} // namespace _   



template <class P>
CharSet <_::not<P>>
operator~ (const CharSet<P> &lhs)
{
   return  CharSet <_::not<P>> (lhs);
}


template <class P>
BaseParser <_::not_p<P>>
operator! (const BaseParser<P> &lhs)
{
   return  BaseParser <_::not_p<P>> (lhs);
}


} // namespace pi
#endif
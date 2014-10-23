#ifndef  _PARSER_ATOMIC_H
#define  _PARSER_ATOMIC_H

#include "../base/Parser.h"

namespace pi {
namespace _ {

template <class Parser>
struct atomic_p: public UnaryExpression <Parser>
{
   typedef  typename Parser::value_type value_type;

   atomic_p (const Parser &parser): Super (parser) {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const 
   {
      BACK_TRACE;
      scanner.ignoreMessages ();
      bool rc = child->parse (scanner, collect);
      scanner.ignoreMessages (false);

      if (rc)  return true;

      MatchFailure (scanner, *child);
      return false;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      child->toString (out, level);
   }      
};

} // namespace _ 


template <class P>
BaseParser <_::atomic_p<P>>  atomic (const BaseParser<P>& parser)
{
   return  BaseParser <_::atomic_p<P>> (parser);
}

} // namespace pi

#endif
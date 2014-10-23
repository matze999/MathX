#ifndef  _PARSER_NOSKIP_
#define  _PARSER_NOSKIP_

#include "../base/Parser.h"

namespace pi {
namespace _ {

template <class Parser>
struct no_skip: public UnaryExpression <Parser, PID::EXPRESSION>
{
   typedef  typename Parser::value_type value_type;

   no_skip (const Parser &parser): UnaryExpression <Parser, PID::EXPRESSION> (parser) {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const 
   {
      BACK_TRACE;
      auto no_skip_scanner = scanner.getNoSkipScanner();
      if (child.parse (no_skip_scanner, collect))
      {
         scanner = no_skip_scanner;
         return true;
      }
      return false;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      writeOperator (out, "no_skip ", level);
   }      
};

} // namespace _ 


template <class P>
BaseParser <_::no_skip<P>>  no_skip (const BaseParser<P>& parser)
{
   return  BaseParser <_::no_skip<P>> (parser);
}

} // namespace pi

#endif
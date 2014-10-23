#ifndef  _IDLE_H_
#define  _IDLE_H_

#include "../base/Parser.h"

namespace pi {
namespace _ {


// parse item but do not consider its value 
template <class Parser>
struct ignore_p: public UnaryExpression <Parser, PID::EXPRESSION>
{
   ignore_p (const Parser &parser): UnaryExpression <Parser, PID::EXPRESSION> (parser) {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const 
   {
      return child.parse (scanner, fn::no_action() );
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      writeOperator (out, "ignore ", level);
   }
};


// parse item but do not move the cursor 
template <class Parser>
struct idle_p: public UnaryExpression <Parser, PID::EXPRESSION>
{
   idle_p (const Parser &parser): UnaryExpression <Parser, PID::EXPRESSION> (parser) {}


   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const 
   {
      Scanner::Iterator  spos = scanner.begin(); 
      if (child.parse (scanner, fn::no_action() ))
      {
         scanner.begin (spos);
         return true;
      }
      return false;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      writeOperator (out, "idle ", level);
   }
};

} // nammespace _


template <class P>
BaseParser <_::ignore_p <P>>  ignore (const BaseParser<P>& parser)
{
   return  BaseParser <_::ignore_p <P>> (parser);
        //_::ignore_p <P> (parser);
}

template <class P>
BaseParser <_::idle_p <P>>  idle (const BaseParser<P>& parser)
{
   return  BaseParser <_::idle_p <P>> (parser);
}


BaseParser <_::idle_p <_::lit_p>>  idle (const char* lit)
{
   return  BaseParser <_::idle_p <_::lit_p>> (_::lit_p(lit));
}


} // namespace pi

#endif
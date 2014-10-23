#ifndef  _PARSER_SYMBOL_H
#define  _PARSER_SYMBOL_H

#include  "literal.h"
#include  "../scanner/Scanner.h"


namespace pi { 


namespace _ {


template <class Value>
class symbol_p: private lit_p
{
public:
   typedef  Value  value_type;

   static const int  rank = PID::Literal;

   symbol_p (const std::string& str, const Value& val): lit_p (str), value (val)
   {}


   template <class Scanner, class Attribute>
   bool parse (Scanner &scanner, Attribute &attribute) const
   {
      if (!lit_p.parse (scanner, attribute))  
         return MatchFailure (scanner);

      mp::assign (collect, value);
      return  MatchSuccess (scanner, data);
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      out << "symbol \"" << data << '"';
   }

private:
   Value  value;
};


} // namespace _



template <class Value>
BaseParser <_::symbol_p <Value>>  
symbol (const std::string* key, const Value& value)
{
   return  BaseParser <_::symbol_p <Value>> (key, value);
}


} // namespace pi
#endif


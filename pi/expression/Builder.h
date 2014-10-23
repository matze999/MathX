#ifndef  _BUILDER_PARSER__
#define  _BUILDER_PARSER__

#include  "rule.h"
#include  "../scanner/Scanner.h"


namespace pi { 


namespace _ {


template <class Base>
class builder_p
{
public:
   static const int  rank = PID::EXPRESSION;

   builder_p ()
   {
      base = static_cast <Base*> (this);
   }

   template <class Scanner, class Attribute>
   bool parse (Scanner &scanner, Attribute &attribute) const
   {
      BACK_TRACE;
      return execute (scanner, attribute);
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      out << "<" << getTypeId (Base) << ">";
   }

private:
   Base *base;
};


} // namespace _



template <class Base>
using make_parser = BaseParser <_::builder_p <Base>>;




} // namespace pi
#endif


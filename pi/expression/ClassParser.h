#ifndef  _PI_CLASSPARSER__
#define  _PI_CLASSPARSER__

#include  "Rule.h"


namespace pi { 


namespace _ {



template <class Base>
class class_p
{
   Base *base;
public:
   static const int  rank = PID::EXPRESSION;

   class_p ()
   {
      base = static_cast <Base*> (this);
   }


   template <class Scanner>
   bool parse (Scanner &scanner) const
   {
      BACK_TRACE;
      if (base->parseImp (scanner))  return true;

      return  RuleFailure (scanner, getTypeId (Base));
   }


   template <class Scanner, class Attribute>
   bool parse (Scanner &scanner, Attribute &attribute) const
   {
      if (!parse (scanner))  return false;

      mp::assign (attribute, *base);
      return true;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      out  << getTypeId (Base);
   }
};




} // namespace _



template <class Base>
using ClassParser = BaseParser <_::class_p <Base>>;




} // namespace pi
#endif



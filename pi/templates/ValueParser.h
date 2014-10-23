#ifndef _VALUE_PARSER_
#define _VALUE_PARSER_

#include <ErrorInfo.h>
#include <StrUtils.h>
#include "../base/Base.h"


namespace pi { 
namespace _ {


template <class Value>
struct value_p
{
   static const int rank = PID::LITERAL;

   value_p (const Value& value): attribute(value)  {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      BACK_TRACE;
      mp::assign (collect, attribute);
      return true;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
       out << "var(" << getTypeId(Value) << ")";
   }

private:
    mutable Value  attribute;
};



template <class Value>
struct ref_p
{
   static const int rank = PID::LITERAL;

   ref_p (Value& value): attribute(value)  {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      BACK_TRACE;
      mp::assign (collect, attribute);
      return true;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
       out << "ref(" << getTypeId(Value) << ")";
   }

private:
    mutable Value&  attribute;
};


} // namespace _



template <class Value>
BaseParser<_::value_p<Value>>  var (const Value& value)
{
   return  BaseParser<_::value_p<Value>> (value);
}

BaseParser<_::value_p<const char*>>  var (const char* value)
{
   return  BaseParser<_::value_p<const char*>> (value);
}


template <class Value>
BaseParser<_::ref_p<Value>>  ref (Value& value)
{
   return  BaseParser<_::ref_p<Value>> (value);
}


} // namespace pi 
#endif 


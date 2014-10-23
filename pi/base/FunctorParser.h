#ifndef _FUNCTOR_PARSER_
#define _FUNCTOR_PARSER_

#include <ErrorInfo.h>
#include "Base.h"
#include "Attribute.h"

namespace pi { 

namespace _ {


template <class Fun, class Parser>
struct functor_p: public UnaryExpression <Parser, PID::EXPRESSION>
{
   functor_p (Fun& fun, const Parser &parser):
      function(fun), UnaryExpression <Parser, PID::EXPRESSION> (parser)  {}


   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      BACK_TRACE;
      typename Parser::value_type  value;
      if (child.parse (scanner, value)) 
      {
         mp::assign (collect, function(value));
         return true;
      }
      return false;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
       writeOperator (out, "", level);
       out << "[" << getTypeId(Fun) << "]";
   }

private:
    Fun  function;
};


template <class Parser>
struct message_p: public UnaryExpression <Parser, PID::EXPRESSION>
{
   message_p (const Message& msg, const Parser &parser):
      message(msg), super (parser)  {}


   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      if (child.parse (scanner, collect)) 
      {
         scanner.getErrorInfo ().addMessage (message);
         return true;
      }
      return false;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      if (level <= WRITEMODE::COMPACT)
         child.toString (out, level);
      else
      {
         writeOperator (out, "", level);
         out << "[" << message << "]";
      }
   }

private:
   Message  message;
};



} // namespace _


} // namespace pi 
#endif 


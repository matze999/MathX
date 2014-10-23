#ifndef _LOG_PARSER_
#define _LOG_PARSER_

#include <ErrorInfo.h>
#include "Base.h"
#include "Attribute.h"

namespace pi { 

namespace _ {


template <class Parser, MSG_TYPE type = MSG_TYPE>
struct log_p: public UnaryExpression <Parser>
{
   log_p (const Parser &parser, const std::string& msg, unsigned c = 0):
      Super (parser), message (msg), code(c)  {}


   template <class Scanner, class Attribute>
   bool parse (Scanner &scanner, Attribute &attribute) const
   {     
      if (child.parse (scanner, attribute))  return true;

      scanner.addMessage (::toString(child), code, message, type);
      scanner.newErrorRecord ();
      return false;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      child.toString (out, level);
   }

private:
    std::string  message;
    unsigned     code;
};


} // namespace _
} // namespace pi 
#endif 


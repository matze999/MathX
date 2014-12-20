#ifndef _BASEPARSER_H_
#define _BASEPARSER_H_


#include <ErrorInfo.h>
#include <meta.h>
#include "Base.h"
#include "RefParser.h"
#include "FunctorParser.h"
#include "LogParser.h"

namespace pi {


template <class P>
class BaseParser: public P
{
public:
   typedef  P  type;
   typedef  BaseParser  Super;


   template <class ...Args>
   BaseParser (Args&&... args): P(std::forward<Args>(args)...)   {}

   BaseParser (const BaseParser&) = default;


   using P::parse;


   template <class Attribute>
   void parse (const char *input, Attribute&  value) const
   {   
      Scanner<> scanner (input);

      if (!parse (scanner, value))
         throw  scanner.getErrorInfo();
   }


   void parse (const char *input) const
   {   
      parse (input, fn::no_action());
   }


   template <class Scanner>
   bool parse (Scanner &scanner) const
   {
      return parse (scanner, fn::no_action ());
   }

   template <class Value>
   BaseParser <_::reference_p <Value, P>>  operator() (Value& value) const
   {
      return _::reference_p <Value, P> (value, *this);
   }

   template <class Fun>
   BaseParser <_::functor_p <Fun, P>>  operator[] (Fun& fun) const
   {
      return _::functor_p <Fun, P> (fun, *this);
   }

   template <class Fun>
   BaseParser <_::functor_p <Fun, P>>  apply (Fun& fun) const
   {
      return _::functor_p <Fun, P> (fun, *this);
   }


   BaseParser <_::log_p <P, MSG_TYPE::WARNING>>  
   warning (const std::string& msg, unsigned code = 0) const
   {
      return  _::log_p <P, MSG_TYPE::WARNING> (*this, msg, code);
   }

   BaseParser <_::log_p <P, MSG_TYPE::ERROR>>  
   error (const std::string& msg, unsigned code = 0) const
   {
      return  _::log_p <P, MSG_TYPE::ERROR> (*this, msg, code);
   }
};


template <class P>
std::ostream& operator<< (std::ostream &out, const BaseParser<P>& parser)  
{ 
   parser.toString (out, WRITEMODE::EXPAND);
   return out; 
}

  


template <class T>
struct is_parser: public mp::is_template_of <BaseParser, T>  {};



} // namespace pi
#endif

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
   BaseParser (Args&&... args): P(args...)   {}

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
std::ostream& operator<< (std::ostream &out, const BaseParser<P> &parser)  
{ 
   parser.toString (out, WRITEMODE::EXPAND);
   return out; 
}

  


using_namespace (::mp)

template <class T>
struct is_parser: public is_template_of <BaseParser, T>  {};


//
//template <class Parser, class Scanner, class Collector>
//typename enable_if <is_void <typename Parser::value_type>::value, bool>::type
//parseImp (const Parser& parser, Scanner &scanner, Collector &collect)
//{
//   return parser.parse (scanner, collect);
//}
//
//
//template <class Parser, class Scanner, class Collector>
//typename enable_if <!is_void<typename Parser::value_type>::value, bool>::type
//parseImp (const Parser& parser, Scanner &scanner, Collector &collect)
//{
//   TRC_DEBUG (pi) << printExpression (parser) << mgo::endl;
//   if (parser.parse (scanner, collect))  return true;
//
//   TRC_DEBUG(pi) << "no match" << data << mgo::endl;
//   scanner.getErrorInfo ().addMessage (NO_MATCH, parser, *NO_MATCH);
//   return false;
//}
//

end_using (::mp)



} // namespace pi
#endif

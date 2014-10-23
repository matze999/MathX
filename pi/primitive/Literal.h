#ifndef _PARSER_LITERAL_H_
#define _PARSER_LITERAL_H_

#include "../base/Parser.h"

namespace pi {

namespace _ {


struct lit_p
{
   typedef  void  value_type;
   enum { rank = PID::LITERAL };

   lit_p (const char *chars): data (chars)  {}


   template <class Scanner>
   bool parse (Scanner &scanner) const
   {
      BACK_TRACE;
      Scanner::Iterator  spos = scanner.begin();

      for (char ch: data)
      {
         if (scanner.empty()  ||  ch != scanner.front())  
         {
            scanner.begin (spos);
            return false;
         }
			scanner.pop_front();
      }
      return  MatchSuccess (scanner, data);
   }


   template <class Scanner, class Attribute>
   bool parse (Scanner &scanner, Attribute&) const
	{
		return  parse (scanner);
	}


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      out << '"' << data << '"';
   }

   std::string  data;   
protected:
};



struct token_p: public lit_p
{
   //typedef  std::string  value_type;
   enum { rank = PID::LITERAL };

   token_p (const char *str): lit_p (str)  {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      if (!lit_p::parse (scanner, collect))  return false;
      
      mp::assign (collect, data);
      return true;
   }
};


} // namespace _


struct lit: public BaseParser<_::lit_p>
{
   lit (const char* str): BaseParser<_::lit_p> (str)  {}
};


struct token: public BaseParser<_::token_p>
{
   token (const char* str = ""): BaseParser<_::token_p> (str)  {}
};


} // namespace pi
#endif


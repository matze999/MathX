#ifndef __PARSER_PHRASE_H
#define __PARSER_PHRASE_H

#include "../base/Attribute.h"
#include "Charclass.h"
#include "../scanner/Scanner.h"
#include <StrUtils.h>


namespace pi { 
namespace _ {


class  phrase_p
{
   char begin_del, end_del;

public:
   typedef  std::string  value_type;
   enum { rank = PID::LITERAL };

   phrase_p (char beg, char end = 0): begin_del (beg), end_del (end? end: beg)  {}

   template <class Scanner, class Collector>
   bool parse (Scanner &one_scanner, Collector &collect) const
   {
      BACK_TRACE;
      pi::Scanner<>  scanner = one_scanner;

      if (scanner.empty() || scanner.front() != begin_del) 
         return MatchFailure (scanner);

      scanner.pop_front();
  
      auto  spos = scanner.begin();
      std::string  attribute;
      while (!scanner.empty())
      {
         if (scanner.front() == '\\')
         {
            scanner.pop_front();
            if (scanner.empty())  break;
         }
         else if (scanner.front() == end_del) 
         {
            mp::assign (collect, attribute);
            scanner.pop_front();
            return MatchSuccess (scanner, begin_del + attribute + end_del);
         }

         attribute += scanner.front();
         scanner.pop_front();
      }

      scanner.begin (spos);
      scanner.addMessage (::toString(*this), 1100, "End of file without end delimiter for a <phrase>");
      return false;
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
       out <<  mgo::pretty_char(begin_del) << "phrase" 
           <<  mgo::pretty_char(end_del);
   }
};


} // namespace _


struct Phrase: public BaseParser<_::phrase_p> 
{
   Phrase(): Super ('"', '"')  {}
   Phrase (char beg, char end): Super (beg, end)  {}


   Phrase  operator() (char del) const
   {
      return Phrase (del, del);
   }

   Phrase  operator() (char beg, char end) const
   {
      return Phrase (beg, end);
   }
};

const Phrase phrase;



} // namespace pi 
#endif 


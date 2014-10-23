#ifndef __PRIMITIVE_WORD_
#define __PRIMITIVE_WORD_

#include "../base/Attribute.h"
#include "Charclass.h"
#include "../scanner/Scanner.h"


namespace pi { 
namespace _ {


struct word_p
{
   typedef  std::string  value_type;
   enum { rank = PID::LITERAL };

   typedef  std::string  value_type;

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      BACK_TRACE;
      auto   base_scanner = scanner.getNoSkipScanner();
      std::string  attribute;

      TRC_SECTION (TL_ERROR)
      {
         if (!pi::ch::alpha.parse (base_scanner, attribute))  
            return MatchFailure (base_scanner);
         while (pi::ch::word.parse (base_scanner, attribute))  {}
      }

      scanner.skip ();
      mp::assign (collect, attribute);
      return MatchSuccess (scanner, attribute);
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      out << "word";
   }
};


} // namespace _


BaseParser<_::word_p>  word;

} // namespace pi 
#endif 


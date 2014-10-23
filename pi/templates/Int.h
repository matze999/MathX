#ifndef _PARSER_INT_H_
#define _PARSER_INT_H_

#include "../base/Parser.h"
#include <numreader.h>
#include <StrUtils.h>

namespace pi {

namespace _ {


struct int_p
{
   typedef  int  value_type;
   enum {rank = PID::PRIMITIVE };

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      BACK_TRACE;
      int value;
      if (!readInteger (scanner, value))  
         return MatchFailure (scanner);
      
      mp::assign (collect, value);
      return  MatchSuccess (scanner, "int");
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      out << "int";
   }
};


struct double_p
{
   typedef  double  value_type;
   enum {rank = PID::PRIMITIVE };

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      BACK_TRACE;
      double value;
      if (!mgo::readDouble (scanner, value)) 
         return MatchFailure (scanner);

      mp::assign (collect, value);

      return MatchSuccess (scanner, "double");
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      out << "double";
   }
};

} // namespace _


const  BaseParser<_::int_p>  int_;
const  BaseParser<_::double_p>  double_;


} // namespace pi
#endif


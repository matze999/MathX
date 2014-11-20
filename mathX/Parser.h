#ifndef __MATHX_PARSER_H
#define __MATHX_PARSER_H


#include <pi/parser.h>
#include <functional>
#include "Function.h"


namespace mathX {



struct RandomNumberParser: public pi::make_parser < RandomNumberParser >
{
   template <class Scanner, class Attribute>
   bool execute (Scanner& scannner, Attribute&  attribute)
   {
      using namespace pi;
      if (!lit ("#").parse (scanner))  return false;
      int lb, ub, len = 0, decimals = 0;
      auto  parser = "[" >> int_(lb) >> "," >> int_(ub) "]"
                   | int_(len) >> -("." >> int_(decimals));

      if (("#" >> parser).parse (scanner))
      {

      }

      Rule <Range>  range_p =  
   }
};


struct  RangeParser: public Range, public pi::make_parser < RangeParser >
{
   RangeParser ()
   {
      start_ = "[" >> int_ (lb) >> "," >> int_ (ub) "]";
   }

   void setup ();
   void conclude ();
};

parser (range) = "[" >> int_(range.lb) >> "," >> int_(range.ub) "]"

} // namespace mathX
#endif 


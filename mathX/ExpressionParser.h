#ifndef __MATHX_PARSER_H
#define __MATHX_PARSER_H


#include <pi/Parser.h>
#include <pi/Operator.h>
#include <pi/expression/ClassParser.h>
#include <functional>
#include "Function.h"


namespace mathX {



template <class Scanner>
class  IntvalParser:
   public Intval, public pi::ClassParser < IntvalParser<Scanner> >
{
public:
   IntvalParser ()
   {
      parser = "[" >> pi::int_ (lb) >> "," >> pi::int_ (ub) >> "]";
   }

   IntvalParser (IntvalParser& other): IntvalParser ()  {}
    

   template <class Scan>
   bool parseImp (Scan& scanner)
   {
      return parser.parse (scanner, fn::ignore);
   }

private:
   pi::Rule0 <Scanner>  parser;
};


template <class Scanner>
class  AccuracyParser:
   public Accuracy, public pi::ClassParser <AccuracyParser<Scanner>>
{
public:
   AccuracyParser ()
   {
      parser = -pi::symbol ("*", true) (exclude_zero)  >> pi::int_ (mantisse) >> -("." >> pi::int_ (decimals));
   }


   template <class Scan>
   bool parseImp (Scan& scanner)
   {
      decimals = 0;
      exclude_zero = false;
      return parser.parse (scanner, fn::ignore);  //.getNoSkipScanner()
   }

private:
   pi::Rule0 <Scanner>  parser;
};



struct  SkipParser: public pi::Rule0<>
{
   SkipParser ()
   {
      static_cast <pi::Rule0<>&> (*this) = 
         pi::ch::space |
         "//" >> pi::char_* pi::ch::eol;
   }

   SkipParser (const SkipParser&): SkipParser ()  {}
};



class ExpressionParser:
   public pi::Grammar <Expression, SkipParser>,
   private pi::op<Expression>
{
public:
   ExpressionParser (Allocator&  allocator): start_rule (equation),
      alloc (allocator),
      equation (allocator),
      term (allocator),
      sum (allocator),
      product (allocator),
      exponent (allocator)
   {
      using namespace pi;


      term = number
         | word
         | "#" >> (intval | accuracy)
         | "(" >> sum >> ")";

      sum = product % (plus | minus);
      product = exponent % (multiply | divide);
      exponent = term % power;

      equation = sum >> -(equal >> sum);
   }

private:
   Allocator&  alloc;
   Rule<Expression>  equation, term;
   Rule<Expression>  sum, product, exponent;

   pi::Operator<Expression> power = { "^", pow };
   pi::Operator<Expression> equal = { "=", mathX::eq };

   IntvalParser<scanner_type> intval;
   AccuracyParser<scanner_type>  accuracy;
};





} // namespace mathX
#endif 


#ifndef  _PARSER_LAZY_SEQUENCE_H_
#define  _PARSER_LAZY_SEQUENCE_H_
       
#include "Sequence.h"
#include "Exclude.h"

namespace pi { 

namespace _ { 


template <class P, size_t min, size_t max, class Q>
struct lazyrep_p : public BinaryExpression <P, Q, PID::LAZY_REPEAT>
{
   lazyrep_p (const P &lhs, const Q &rhs): Super(lhs, rhs)  {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      Scanner::Iterator  spos = scanner.begin();

      sizea_t count = 0;
      while (count < min)
      {
         if (right.parse (scanner, fn::no_action())  ||
             !left.parse (scanner, collect))
         {
            scanner.begin (spos);
            return false;
         }
         ++count;
      }

      while (count <= max)
      {
         if (right.parse (scanner, collect))  return true;
         if (!left.parse (scanner, collect))  break;
         ++count
      }
           
      scanner.begin (spos);
      return false;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
         writeOperator (out, "rep", level);
   }
};



template <class P, class Q, class = void>
struct lazyopt_p : public BinaryExpression <P, Q, PID::LAZY_REPEAT>
{
   lazyopt_p (const P &lhs, const Q &rhs): Super(lhs, rhs)  {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      Scanner::Iterator  spos = scanner.begin();

      if (right.parse (scanner, collect)  ||
         (left.parse (scanner, collect)  &&  right.parse (scanner, collect)) ) 
         return true;
           
      scanner.begin (spos);
      return false;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      writeOperator (out, "- ", level);
   }
};



template <class P, class Q, class = void>
struct lazystar_p : public BinaryExpression <P, Q, PID::LAZY_REPEAT>
{
   lazystar_p (const P &lhs, const Q &rhs): Super(lhs, rhs)  {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      Scanner::Iterator  spos = scanner.begin();

      do
      {
         if (right.parse (scanner, collect))  return true;
      } 
      while (left.parse (scanner, collect));
           
      scanner.begin (spos);
      return false;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
         writeOperator (out, "* ", level);
   }
};



template <class P, class Q, class = void>
struct lazyplus_p : public BinaryExpression <P, Q, PID::LAZY_REPEAT>
{
   lazyplus_p (const P &lhs, const Q &rhs): Super(lhs, rhs)  {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      Scanner::Iterator  spos = scanner.begin();

      if (!right.parse (scanner, fn::no_action()) &&
           left.parse (scanner, collect))  
      {
         do 
         {
            if (right.parse (scanner, collect))  return true;
         }
         while (left.parse (scanner, collect));
      }
           
      scanner.begin (spos);
      return false;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
         writeOperator (out, "+ ", level);
   }
};


} // namespace _


template <class P, class Q>
typename mp::make_parser <_::lazyopt_p, P, Q>::type
operator- (P &lhs, Q &rhs)
{
   return  mp::make_parser <_::lazyopt_p, P, Q>::execute (lhs, rhs);
}



template <class P, class Q>
typename mp::make_parser <_::lazystar_p, P, Q>::type
operator* (const P &lhs, const Q &rhs)
{
   return  mp::make_parser <_::lazystar_p, P, Q>::execute (lhs, rhs);
}


template <class P, class Q>
typename  mp::make_parser <_::lazyplus_p, P, Q>::type
operator+ (P &lhs, Q &rhs)
{
   return  mp::make_parser <_::lazyplus_p, P, Q>::execute (lhs, rhs);
}



} // namespace pi
#endif

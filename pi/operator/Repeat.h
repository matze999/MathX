#ifndef  _PARSER_REPEAT_H_
#define  _PARSER_REPEAT_H_

#include "MakeParser.h"


namespace pi { 
namespace _ {


template <class P>
struct optional_p: public UnaryExpression <P, PID::REPEAT>
{
   optional_p (const P& parser): Super (parser)  {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      child.parse (scanner, collect);
      scanner.getErrorInfo().clearRecord();
      return  true;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      writeOperator (out, "-", level);
   }
};



template <class P>
struct kleenestar_p: public UnaryExpression <P, PID::REPEAT>
{
   kleenestar_p (const P& parser): Super (parser)  {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      while (child.parse (scanner, collect))  
      {
         if (scanner.empty())  break;
      }
      scanner.getErrorInfo().clearRecord();
      return  true;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      writeOperator (out, "*", level);
   }
};



template <class P>
struct plus_p: public UnaryExpression <P, PID::REPEAT>
{
   plus_p (const P& parser): Super (parser)  {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      if (!child.parse(scanner, collect))
      {
         return false;
      }

      while (child.parse (scanner, collect))  {}
      scanner.getErrorInfo().clearRecord();
      return  true;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      writeOperator (out, "+", level);
   }
};


// attention! repeat does not work fine. The class parse more than needed, i.e the collector 
// can be called more times as needed.
template <class P, size_t  min, size_t max = min>
class repeat_p: public UnaryExpression <P, PID::REPEAT>
{
   mutable size_t  count;
public:
   repeat_p (const P& parser): Super (parser), count(0)  {}


   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      if (child.parse (scanner, collect))
      {
         ++count;
         while (!scanner.empty() && child.parse (scanner, collect))
            ++count;
      }
            
      return  mgo::in (count, min, max);
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      if (min == 0)
         out <<  (max == 1? '-': '*');
      else if (min == 1)
         out << '+';
      else
         out << "rep ";

      if (P::rank > PID::REPEAT)  
         out << '(' << String (child, level) << ")";
      else
         out << String (child, level);
   }

   size_t  getCount() const
   {
      return count;
   }
};

} // namespace _


//  optional

template <class P>
BaseParser <_::optional_p<P>>
operator- (const BaseParser<P> &lhs)
{
   return // BaseParser <_::optional_p<P>> ((const P&)lhs);
         _::optional_p<P> (lhs);
}



//  any times (kleene operator)

template <class P>
BaseParser <_::kleenestar_p<P>>
operator* (const BaseParser<P> &lhs)
{
   return  //BaseParser <_::kleenestar_p<P>> ((const P&)lhs);
           _::kleenestar_p<P> (lhs);
}



//  one or more times

template <class P>
BaseParser <_::plus_p<P>>
operator+ (const BaseParser<P> &lhs)
{
   return // BaseParser <_::plus_p<P>> ((const P&)lhs);
           _::plus_p<P> (lhs);
}


//  explicit repeat times

template <size_t min, size_t max, class T>
BaseParser <_::repeat_p<T, min, max>>
rep (const BaseParser<T> &lhs)
{
   return  _::repeat_p<T, min, max> (lhs);
}





} // namespace pi
#endif

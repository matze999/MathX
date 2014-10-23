#ifndef _BARSER_BACKREF_H_
#define _BARSER_BACKREF_H_

#include "../base/parser.h"
#include "../primitive/Literal.h"

/*
Warning!  The backref is only working with string like parser

Usage Example:

   auto  begin = "<" >> pi::token >> ">";
   auto  end   = "</" >> pi::token >> ">";
   content = ...

   group = (_1 = begin) >> content >> (_1 == end);

   to do so 'begin' and 'end' have to parse to the same value.

or
         
   group = (_1 = "<" >> pi::word >> ">") >> content >> (_1 == end)

*/


namespace pi {
namespace _ {


struct  ref_type: public token
{
   ref_type(): pos (++MaxPosition())  {}

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      out << "_"<< pos;
   }

   int pos;
   

   static int& MaxPosition()
   {
      static int pos = 0;
      return pos;
   }
};



template <>
struct ref_to <ref_type>
{
   typedef const ref_type&  type;
};



template <class Parser>
struct ref_assigner: public BinaryExpression <ref_type, Parser>
{
   ref_assigner (const ref_type& ref, const Parser& parser):
   Super (ref, parser)  {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      BACK_TRACE;
      std::string& data = const_cast <std::string&> (left.data);
      data.clear ();

      if (!right.parse (scanner, data))  return false;
      mp::assign (collect, data);
      return true;
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      out << "(_"<< left.pos << "= ";
      if (level == WRITEMODE::MINIMAL || level == WRITEMODE::COMPACT)
         right.toString (out, WRITEMODE::MINIMAL);
      else
         right.toString (out, level);
      out << ')';
   }
};



template <class Parser>
struct ref_equal: public BinaryExpression <ref_type, Parser>
{
   ref_equal (const ref_type& ref, const Parser& parser): 
      Super(ref, parser)  {}

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      BACK_TRACE;
      std::string  result;
      if (!right.parse (scanner, result))  return false;

      if (left.data != result)
      {
         std::stringstream out;
         out << "Expect '" << left.data << "' here";
         scanner.addMessage (::toString (right, WRITEMODE::MINIMAL), 0, out.str ());
         return false;
      }

      mp::assign (collect, result);
      return true;
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      out << "(_"<< left.pos << " == ";
      if (level == WRITEMODE::MINIMAL || level == WRITEMODE::COMPACT)
         right.toString (out, WRITEMODE::MINIMAL);
      else
         right.toString (out, level);
      out << ')';
   }
};

} // namespace _


namespace backref {

struct  RefType: public BaseParser <_::ref_type>
{

   template <class P>
   BaseParser<_::ref_assigner<P>> operator= (const BaseParser<P>& parser) const 
   {
      return  BaseParser<_::ref_assigner<P>> (*this, parser);
   }
};


template <class P>
BaseParser<_::ref_equal<P>> 
operator== (const RefType& ref, const BaseParser<P>& parser)
{
   return  BaseParser<_::ref_equal<P>> (ref, parser);
}


template <class P>
BaseParser<_::ref_equal<P>> 
operator== (const RefType&& ref, const BaseParser<P>& parser)
{
   static_assert (0, "Do not use temporary backref types");
}


template <class P>
BaseParser<_::ref_equal<P>> 
operator== (const BaseParser<P>& parser, const RefType& ref)
{
   return  BaseParser<_::ref_equal<P>> (ref, parser);
}


const RefType _1, _2, _3;

} // namespace backref
} // namespace pi
#endif


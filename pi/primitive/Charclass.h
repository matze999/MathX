#ifndef  _CHARCLASS_H_
#define  _CHARCLASS_H_


#include "StaticChar.h"
#include "../base/Parser.h"



namespace pi {
namespace ch {



template <class T>
struct  char_p
{
   typedef  char  value_type;
   enum { rank = PID::LITERAL };


	template <class Scanner, class Attribute>
   bool parse (Scanner &scanner, Attribute &attribute) const
   {
      return  rho::parseChar <T> (scanner, attribute);
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      out << '['; 
		T::toStream (out);
		out << ']';
   }
};



template <class T>
using  CharClass = BaseParser<char_p<T>>;



template <class T>
CharClass <rho::not<T>>
operator~ (const CharClass<T>& ch)
{
   return  CharClass <not<T>> ();
}


template <class P, class Q>
CharClass <rho::or <P, Q>>
operator| (CharClass<P> lhs, CharClass<Q> rhs)
{
   return  CharClass <rho::or <P, Q>> ();
}


using WhiteSpace = CharClass <rho::whitespace>;
const WhiteSpace  space;

const CharClass <rho::blank>		blank;
const CharClass <rho::lower>		lower;
const CharClass <rho::upper>		upper;
const CharClass <rho::digit>		digit;

const CharClass <rho::alpha>		alpha;
const CharClass <rho::alphanum>  alphanum;
const CharClass <rho::word>		word;



struct  eol_p
{
   typedef  char  value_type;
   static const int rank = PID::LITERAL;

   template <class Scanner, class Attribute>
   bool parse (Scanner &scanner, Attribute &attribute) const
   {
		rho::parseChar <rho::chars <'\r'>> (scanner, fn::ignore);

		return  rho::parseChar <rho::chars <'\n'>> (scanner, attribute);
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      out << "eol"; 
   }   
};


const BaseParser<eol_p>  eol;


} // namespace ch


using WhiteSpace = ch::WhiteSpace;

} // namespace pi 
#endif 


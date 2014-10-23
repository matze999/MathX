#ifndef _PARSER_STATICCHAR_H_
#define _PARSER_STATICCHAR_H_

//#include <StrUtils.h>


namespace pi {
namespace rho {


struct any_char
{
   static bool  match (char in)
   {
      return true;
   }
};



template <char ch = 0, char ...tail>
struct  chars
{ 
   static bool  match (char in)
   {
      return in == ch || chars <tail...>::match (in);
   }

	static  void  toStream (std::ostream &out)
	{
		out << ch;
		chars <tail...>::toStream (out);
	}
};


template <>
struct chars<>
{
   static  bool  match (char in)						{ return false; }
	static  void  toStream (std::ostream &out)	{}
};



template <char lb, char ub>
struct  range
{
   static bool  match (char ch) 
   {
      return  lb <= ch  &&  ch <= ub;
   }

	static  void  toStream (std::ostream &out)
	{
		out << lb << '-' << ub;
	}
};



template <class T>
struct  not
{
   static bool  match (char ch) 
   {
      return  !T::match(ch);
   }


	static  void  toStream (std::ostream &out)
	{
		out << '~';
		T::toStream (out);
	}
};



template <class P, class Q>
struct  or
{
   static bool  match (char ch) 
   {
      return  P::match(ch)  ||  Q::match (ch);
   }

	static  void  toStream (std::ostream &out)
	{
		P::toStream (out);
		Q::toStream (out);
	}
};



using  blank = chars<' ','\t'>;
using  whitespace = chars<' ','\n', '\t', '\r'>;
using  lower = range <'a', 'z'>;
using  upper = range <'A', 'Z'>;
using  digit = range <'0', '9'>;

using  alpha		= or <lower, upper>;
using  alphanum	= or <alpha, digit>;
using  word			= or <alphanum,  chars<'_'>>;



template <class T, class Scanner, class Attribute>
bool parseChar (Scanner &scanner, Attribute &attribute)
{
   if ( scanner.empty() || !T::match (scanner.front()) )
		return  MatchFailure (scanner);

   char  token = scanner.front();
   scanner.pop_front();

   mp::assign (attribute, token);
   return  MatchSuccess (scanner, mgo::toString (token));
}


} // namespace rho
} // namespace pi
#endif

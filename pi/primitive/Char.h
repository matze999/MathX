#ifndef _PARSER_CHAR_H_
#define _PARSER_CHAR_H_

#include "../base/Parser.h"
#include <StrUtils.h>


namespace pi {
namespace _ {


struct  one
{ 
   one (char in): ch(in)  {}

   bool  match (char in) const
   {
      return ch == in;
   }

   void toStream (std::ostream &out) const
   {
       out << '\'' << mgo::pretty_char (ch) << '\'';
   }

protected:
   char ch;
};


struct  many
{
   many (const char *chars) 
   {
		assert (chars);
      begin = chars;
      end   = chars + strlen(chars);
   }

   void toStream (std::ostream &out) const
   {
      out << '[';
      mgo::pretty_print (out, begin);
      out << ']';
   }

   bool  match (char ch) const
   {
      return  std::find (begin, end, ch) != end;
   }

protected:
   const char*  begin;
   const char*  end;
};



struct  interval
{
   interval (char lower, char upper = 0): lb(lower), ub(upper)  {}

   void  toStream (std::ostream &out) const
   {
      out << '[' << lb << '-' << ub << ']';
   }

   bool  match (char ch) const
   {
      return  ch >= lb  &&  ch <= ub;
   }

protected:
   char lb, ub;
};



struct  any_char
{
	bool match (int ch) const
   {
      return true;
   }


   void toStream (std::ostream &out) const
   {
      out << '.'; 
   }
};



template <class T>
struct  not
{
	not (T other): child (other)  {}

   bool  match (char ch) const
   {
      return  !child.match(ch);
   }


	void  toStream (std::ostream &out) const
	{
		out << '~';
		child.toStream (out);
	}
protected:
	T  child;
};




template <class Base>
struct char_p: public Base, public add_plain_parser <char_p<Base>, char>
{
   using  value_type = char;
   enum { rank = PID::LITERAL };
 

	template <class ...Args>
	char_p (Args&&... args): Base(args...)  {}

   using  add_plain_parser <char_p<Base>, char>::parse;

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const 
   {
      BACK_TRACE;  
      if (scanner.empty ()  ||  !match (scanner.front()))
         return  MatchFailure (scanner);

      char  token = scanner.front();
      scanner.pop_front();

      mp::assign (collect, token);
      return  MatchSuccess (scanner, mgo::toString (token));
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      toStream (out);
   }
};




template <class Base>
struct notchar_p: public Base
{
   typedef  char  value_type;
   enum { rank = PID::LITERAL };

   notchar_p (const Base&  base): Base(base)  {}


   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      BACK_TRACE;
      if (scanner.empty ()  ||  match (scanner.front()))
         return  MatchFailure (scanner);

      char  token = scanner.front();
      scanner.pop_front();

      mp::assign (collect, token);
      return  MatchSuccess (scanner, mgo::toString(token));
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      out << '~'; 
      Base::toString (out);
   }
};


} // namespace _



template <class T>
using  CharSet = BaseParser <_::char_p<T>>;



using  Char = CharSet <_::many>;
using  CharRange = CharSet <_::interval>;
using  OneChar = CharSet <_::one>;



class AnyChar: public CharSet<_::any_char>
{
public:


   Char  operator() (const char *chars) const
   {
      return  Char (chars);
   }

   CharRange  operator() (char lb, char ub) const
   {
      return  CharRange (lb, ub);
   }

   OneChar  operator() (char ch) const
   {
      return  OneChar (ch);
   }
};



const AnyChar char_;




} // namespace pi
#endif

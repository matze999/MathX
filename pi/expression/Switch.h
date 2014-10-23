#ifndef  _PARSER_SWITCH__
#define  _PARSER_SWITCH__


#include "../base/Parser.h"
#include "../base/Attribute.h"
#include <meta/typetraits.h>
#include <trival.h>



namespace pi { 

namespace _ {


class case_tag {};

const trival unprocessed = undef;


template <class T>
typename mp::enable_if <mp::is_arithmetic <T>::value>::type
quoteValue (std::ostream& out, T value)
{
   out << value;
}

void quoteValue (std::ostream& out, char value)
{
   out << '\'' << value << '\'';
}

void quoteValue (std::ostream& out, const std::string& value)
{
   out << '"' << value << '"';
}




template <class Value, class Parser>
class case_p: public UnaryExpression <Parser, PID::EXPRESSION>, public case_tag
{
   Value  value;
public:
   case_p (const Value& val, const Parser &p): Super(p), value (val)  {}


   template <class Scanner, class Attribute>
   trival  parse (Scanner &scanner, Attribute&  attribute, const Value& result) const
   {
      BACK_TRACE;
      TRC_INDENT_SECTION (4, parser);

      if (value != result)   return unprocessed;

      if (child.parse (scanner, attribute))  return true;

      return false;
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      out << "case ";
      quoteValue (out, value);
      if (level == WRITEMODE::MINIMAL)
         {}
      else if (level == WRITEMODE::COMPACT)
         writeOperator (out, ":", WRITEMODE::MINIMAL);
      else
         writeOperator (out, ":", level);
   }
};



template <class Parser>
class default_p: public UnaryExpression <Parser, PID::EXPRESSION>, public case_tag
{
public:
   default_p (const Parser &parser): Super(parser)  {}


   template <class Scanner, class Attribute, class Value>
   bool  parse (Scanner &scanner, Attribute& attribute, const Value&) const 
   {
      BACK_TRACE;
      TRC_INDENT_SECTION (4, parser);

      if (child.parse (scanner, attribute))
         return true;

      //scanner.getErrroInfo().addMessage (PARSING_ERROR 
      //         "switch:", "default parser could not be matched");
      return false;
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      if (level == WRITEMODE::MINIMAL)
         out << "default";
      else  if (level == WRITEMODE::COMPACT)
         writeOperator (out, "default:", WRITEMODE::MINIMAL);
      else
         writeOperator (out, "default:", level);
   }
};



template <class P, class Q>
class case_list: public BinaryExpression <P, Q, PID::EXPRESSION>, public case_tag
{
public:
   case_list (const P& lhs, const Q &rhs): Super (lhs, rhs)  {}


   template <class Scanner, class Attribute, class Value>
   trival  parse (Scanner &scanner, Attribute&  attribute, const Value& result) const 
   {
      trival  rc = left.parse (scanner, attribute, result);

      return  rc != unprocessed? rc: 
                    right.parse (scanner, attribute, result);
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      left.toString (out, level);
      out << "; ";
      right.toString (out, level);
   }
};


template <class Parser, class CaseList>
class switch_p: public BinaryExpression <Parser, CaseList, PID::EXPRESSION>
{
public:
   switch_p (const Parser& lhs, const CaseList &rhs): Super (lhs, rhs)  {}


   template <class Scanner, class Attribute>
   bool  parse (Scanner &scanner, Attribute&  attribute) const 
   {
      BACK_TRACE;
      Parser::value_type  result;
      if (!left.parse (scanner, result))  
         return MatchFailure (scanner, left);

      if (right.parse (scanner, attribute, result))  return true;


      std::stringstream out;
      out << "'" << result << "' is not expected here!";
      scanner.addMessage("switch", MATCH_FAILURE, out.str());
      return false;
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      out << "switch (";
      if (level == WRITEMODE::MINIMAL)
      {
         left.toString (out, WRITEMODE::MINIMAL);
         out << ')';
         return;
      }
      else if (level == WRITEMODE::COMPACT)
         left.toString (out, WRITEMODE::MINIMAL);
      else
         left.toString (out, level);

      out << ")[";
      if (level == WRITEMODE::COMPACT)
         right.toString (out, WRITEMODE::MINIMAL);
      else
         right.toString (out, level);
      out << ']';
   }
};



} // namespace _


using_namespace (::mp);


template <class Parser>
class  _switch_helper
{
   typename ref_to <Parser>::type  switch_parser;
public:

   _switch_helper (const Parser& parser): switch_parser(parser)  {}

   template <class CaseList>
   BaseParser<_::switch_p <Parser, CaseList>>
   operator[] (const CaseList& list)
   {
      return  BaseParser<_::switch_p <Parser, CaseList>> (switch_parser, list);
   }
};


template <class Parser>
_switch_helper<Parser>  switch_ (const Parser& parser)
{
   return  _switch_helper<Parser> (parser);
}




template <class Value>
class _case_helper
{
   Value value;
public:
   _case_helper (const Value& val): value(val)  {}


   template <class P>
   _::case_p <Value, P>
   operator[] (const BaseParser<P>& parser)
   {
      return   _::case_p <Value, P> (value, parser);
   }

   _::case_p <Value, lit>  operator[] (const char* str)
   {
      return   _::case_p <Value, lit> (value, str);
   }
};


template <class Value>
_case_helper<Value>  case_ (const Value& val)
{
   return _case_helper<Value> (val);
}

_case_helper<std::string>  case_ (const char* val)
{
   return  _case_helper<std::string> (val);
}



class default_helper
{
public:
   template <class P>
   _::default_p <P>  operator[] (const BaseParser<P>& parser) const
   {
      return  _::default_p <P> (parser);
   }


   _::default_p <lit>  operator[] (const char* str) const
   {
      return   _::default_p<lit> (str);
   }
};

const default_helper  default_;



template <class P, class Q>
typename enable_if 
<
   and <is_base_of <_::case_tag, P>, is_base_of <_::case_tag, Q>>::value,
   _::case_list <P, Q>
>::type  
operator, (const P& lhs, const Q& rhs)
{
   return _::case_list <P, Q> (lhs, rhs);
}

end_using(mp);
} // namespace pi
#endif

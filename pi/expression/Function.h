#ifndef  _PARSER_FUNCTION_H
#define  _PARSER_FUNCTION_H

#include <functional>
#include <memory>
#include "../primitive/Literal.h"


namespace pi {
namespace _ {


template <class Signatur>  
struct  function_p
{
   static const int rank = PID::FUNCTION;
   using  value_type = const std::function <Signatur>;


   template <class Func>
   function_p (const char *name, const Func& fun
               const char *left = "(", const char* right = ")")
      : func (fun), name (name), open_bracket (left), close_bracket (right)  {}


   void  parse ();

   value_type  func;
   lit_p  name, open_bracket, close_bracket;
};




template <class Fun, class P, class Q = void, class S = void>
struct  formula_p:
{
   static const int rank = PID::EXPRESSION;

   using value_type = typename Fun::value_type::result_type;

   formula_p (const Fun& fun, const P& a1, const Q& a2, const S& a3)
      : func(fun), arg1(a1), arg2(a2), arg3(a3)  {}


   template <class Scanner, class Attribute>
   bool parse (Scanner &scanner, Attribute &attribute) const
   {
      BACK_TRACE;
      Scanner::Iterator  spos = scanner.begin ();

      if (!func.name.parse (scanner))  return false;

      while (true)
      {
         if (!func.open_bracket.parse (scanner)) break;

         auto val1 = arg1.parse (scanner);
         if (!val1)  break;

         auto val2 = arg2.parse (scanner);
         if (!val2)  break;

         auto val3 = arg3.parse (scanner);
         if (!val3)  break;

         if (!func.close_bracket.parse (scanner)) break;

         mp::assign (attribute, func.func (*val1, *val2, *val3));
         return true;
      }

      scanner.begin (spos);
      return false;
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      func.name.toString (out);
      func.open_bracket.toString (out);

      arg1.toString (out, level);
      out << ", ";
      arg2.toString (out, level);
      out << ", ";
      arg3.toString (out, level);

      func.close_bracket.toString (out);
   }


   typename  ref_to<Fun>::type  func;
   typename  ref_to<P>::type   arg1;
   typename  ref_to<Q>::type   arg2;
   typename  ref_to<S>::type   arg3;
};


// Specialisation for 2 arguments
template <class Fun, class P, class Q>
struct  formula_p <Fun, P, Q>:
{
   formula_p (const Fun& fun, const P& a1, const Q& a2)
      : func(fun), arg1(a1), arg2(a2)  {}


   template <class Scanner, class Attribute>
   bool parse (Scanner &scanner, Attribute &attribute) const
   {
      BACK_TRACE;
      Scanner::Iterator  spos = scanner.begin ();

      if (!func.name.parse (scanner))  return false;

      while (true)
      {
         if (!func.open_bracket.parse (scanner)) break;

         auto val1 = arg1.parse (scanner);
         if (!val1)  break;

         auto val2 = arg2.parse (scanner);
         if (!val2)  break;

         if (!func.close_bracket.parse (scanner)) break;

         mp::assign (attribute, func.func (*val1, *val2));
         return true;
      }

      scanner.begin (spos);
      return false;
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      func.name.toString (out);
      func.open_bracket.toString (out);

      arg1.toString (out, level);
      out << ", ";
      arg2.toString (out, level);

      func.close_bracket.toString (out);
   }

   typename  ref_to<Fun>::type  func;
   typename  ref_to<P>::type   arg1;
   typename  ref_to<Q>::type   arg2;
};


// Specialisation for 1 argument
template <class Fun, class P>
struct  formula_p <Fun, P>:
{
   formula_p (const Fun& fun, const P& a1): func(fun), arg1(a1)  {}


   template <class Scanner, class Attribute>
   bool parse (Scanner &scanner, Attribute &attribute) const
   {
      BACK_TRACE;
      Scanner::Iterator  spos = scanner.begin ();

      if (!func.name.parse (scanner))  return false;

      while (true)
      {
         if (!func.open_bracket.parse (scanner)) break;

         auto val1 = arg1.parse (scanner);
         if (!val1)  break;

         if (!func.close_bracket.parse (scanner)) break;

         mp::assign (attribute, func.func (*val1));
         return true;
      }

      scanner.begin (spos);
      return false;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      func.name.toString (out);
      func.open_bracket.toString (out);
      arg1.toString (out, level);
      func.close_bracket.toString (out);
   }

   typename  ref_to<Fun>::type  func;
   typename  ref_to<P>::type   arg1;
};

} // nammespace _



template <class Signatur>
class Function: public BaseParser < _::function_p<Signatur>>
{
public:
   template <class Func>
   Function (const char *name, const Func& fun, 
             const char* left = "(", const char* right = ")"):  
      Super (name, fun, lb)  {}


   template <class P>
   BaseParser<_::formula_p < _::function_p<Signatur>, P> 
   operator()  (const BaseParser<P>& arg)  const
   {
      static_assert0 (function_traits<Signatur>::arity == 1);
      return  BaseParser <_::formula_p < _::function_p<Signatur>, P> (*this, arg);
   }


   template <class P, class Q>
   BaseParser<_::formula_p < _::function_p<Signatur>, P, Q> 
   operator()  (const BaseParser<P1>& arg, const BaseParser<Q>& arg2)  const
   {
      static_assert0 (function_traits<Signatur>::arity == 2);

      return  BaseParser <_::formula_p <_::function_p<Signatur>, P, Q> 
                         (*this, arg1, arg2);
   }


   template <class P, class Q, class S>
   BaseParser<_::formula_p < _::function_p<Signatur>, P, Q, S> 
   operator()  (const BaseParser<P1>& arg, 
                const BaseParser<Q>& arg2,
                const BaseParser<S>& arg3)  const
   {
      static_assert0 (function_traits<Signatur>::arity == 3);

      return  BaseParser <_::formula_p <_::function_p<Signatur>, P, Q, S> 
                         (*this, arg1, arg2, arg3);
   }

};



} // namespace pi

#endif
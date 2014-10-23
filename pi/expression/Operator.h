#ifndef  _PARSER_OPERATOR_H
#define  _PARSER_OPERATOR_H

#include <functional>
#include <memory>
#include "../primitive/Literal.h"
#include "../operator/Sequence.h"
#include "../operator/Alternative.h"
#include "../operator/Repeat.h"


namespace pi {
namespace _ {


template <class Signatur, int type = PID::OPERATOR>
struct operator_p: public lit_p
{
   static const int rank = type;
   using  value_type = const std::function <Signatur>;


   template <class Func>
   operator_p (const char *name, const Func& fun): lit_p (name), func (fun)  {}


   template <class Scanner>
   value_type*  parse (Scanner& scanner) const
   {
      if (!lit_p::parse (scanner))  return  nullptr;

      return  &func;
   }

   value_type  func;
};



template <class P, class Op, class Q>
struct and_p <and_p <P, Op>, Q, enable_if_t<Op::rank == PID::OPERATOR>>
   : public  BinaryExpression <and_p <P, Op>, Q, PID::AND>
{
   using  value_type = typename Op::value_type::result_type;

   and_p (const and_p<P, Op>& lhs, const Q& rhs): Super (lhs, rhs)  {}


   template <class Scanner, class Attribute>
   bool parse (Scanner &scanner, Attribute &attribute) const
   {
      BACK_TRACE;
      Scanner::Iterator  spos = scanner.begin ();

      auto lval = left.left.parse (scanner);
      if (!lval) return false;

      while (true)
      {
         auto func = left.right.parse (scanner);
         if (!func) break;

         auto rval = right.parse (scanner);
         if (!rval) break;

         mp::assign (attribute, (*func) (*lval, *rval));
         return true;
      }

      scanner.begin (spos);
      return false;
   }


   template <class Scanner>
   unique_ptr<value_type> parse (Scanner& scanner) const
   {
      auto  attribute = std::make_unique <value_type> ();

      if (!parse (scanner, *attribute))
         attribute.release ();

      return attribute;
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      writeOperator (out, " ", level);
   }
};



template <class P, class Op, class Q>
struct and_p <P, optional_p<and_p <Op, Q>>, enable_if_t<Op::rank == PID::OPERATOR>>
   : public  BinaryExpression <P, optional_p<and_p <Op, Q>>, PID::AND>
{
   using  value_type = typename Op::value_type::result_type;

   and_p (const P& lhs, const optional_p<and_p <Op, Q>>& rhs): Super(lhs, rhs)  {}


   template <class Scanner, class Attribute>
   bool parse (Scanner &scanner, Attribute &attribute) const
   {
      BACK_TRACE;
      Scanner::Iterator  spos = scanner.begin ();

      auto lval = left.parse (scanner);
      if (!lval) return false;

      auto func = right.child.left.parse (scanner);
      if (!func) // func is optional => parse() is successful
      {
          mp::assign (attribute, *lval);
          return true;
      }

      auto rval = right.child.right.parse (scanner);
      if (rval)
      {
         mp::assign (attribute, (*func) (*lval, *rval));
         return true;
      }
      else
      {
         scanner.begin (spos);
         return false;
      }
   }

   template <class Scanner>
   unique_ptr<value_type> parse (Scanner& scanner) const
   {
      auto  attribute = std::make_unique <value_type> ();

      if (!parse (scanner, *attribute))
         attribute.release ();

      return attribute;
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      writeOperator (out, " ", level);
   }
};




template <class P, class Q>
struct or_p <P, Q, 
   enable_if_t<P::rank == PID::OPERATOR && Q::rank == PID::OPERATOR>>
   : public BinaryExpression <P, Q, PID::OPERATOR>
{
   using  value_type = typename P::value_type;

   or_p (const P &lhs, const Q &rhs): Super (lhs, rhs)  {}
      
   template <class Scanner>
   typename value_type*  parse (Scanner &scanner) const
   {
      value_type*  func = left.parse (scanner);
      if (func)  return func;

      func = right.parse (scanner);
      if (!func)  return nullptr;

      scanner.getErrorInfo().clearRecord();
      return func;
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      writeOperator (out, " | ", level);
   }
};



template <class P, class Op>
struct list_p <P, Op, enable_if_t<Op::rank == PID::OPERATOR>>
   : public BinaryExpression <P, Op, PID::LIST>
{
   using  value_type = typename Op::value_type::result_type;

   list_p (const P &lhs, const Op &rhs): Super (lhs, rhs)  {}
      
   template <class Scanner, class Attribute>
   bool parse (Scanner &scanner, Attribute &attribute) const
   {
      BACK_TRACE;

      auto  spos = scanner.begin ();
      auto  lval = left.parse (scanner);
      if (!lval)  return false;

      while (true)
      { 
         auto func = right.parse (scanner);
         if (!func)  break;
      
         auto rval = left.parse (scanner);
         if (!rval)
         {
            scanner.begin (spos);
            return false;
         }

         *lval = (*func) (*lval, *rval);
      }
      mp::assign (attribute, *lval);
      return true;
   }


   template <class Scanner>
   unique_ptr<value_type> parse (Scanner& scanner) const
   {
      auto  attribute = std::make_unique <value_type> ();

      if (!parse (scanner, *attribute))
         attribute.release ();

      return attribute;
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      writeOperator (out, "%", level);
   }
};



//*** unary operator functionality

template <class P, class Op>
struct and_p <P, Op, enable_if_t <Op::rank == PID::UNARY_OPERATOR>>
   : public  BinaryExpression <P, Op, PID::AND>
{
   and_p (const P& lhs, const Op& rhs): Super (lhs, rhs)  {}


   template <class Scanner, class Attribute>
   bool parse (Scanner &scanner, Attribute &attribute) const
   {
      BACK_TRACE;
      Scanner::Iterator  spos = scanner.begin ();

      auto lval = left.parse (scanner);
      if (!lval) return false;

      auto func = right.parse (scanner);
      if (func)
      {
         mp::assign (attribute, (*func) (*lval));
         return true;
      }
      else
      {
         scanner.begin (spos);
         return false;
      }
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      writeOperator (out, "", level);
   }
};



template <class Op, class Q>
struct and_p <Op, Q, enable_if_t <Op::rank == PID::UNARY_OPERATOR>>
   : public  BinaryExpression <Op, Q, PID::AND>
{
   and_p (const Op& lhs, const Q& rhs): Super (lhs, rhs)  {}


   template <class Scanner, class Attribute>
   bool parse (Scanner &scanner, Attribute &attribute) const
   {
      BACK_TRACE;
      Scanner::Iterator  spos = scanner.begin ();

      auto func = left.parse (scanner);
      if (!func) return false;

      auto rval = right.parse (scanner);
      if (rval)
      {
         mp::assign (attribute, (*func) (*rval));
         return true;
      }
      else
      {
         scanner.begin (spos);
         return false;
      }
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      writeOperator (out, "", level);
   }
};



template <class P, class Q>
struct or_p <P, Q, 
   enable_if_t <P::rank == PID::UNARY_OPERATOR && Q::rank == PID::UNARY_OPERATOR>>
   : public BinaryExpression <P, Q, PID::UNARY_OPERATOR>
{
   or_p (const P &lhs, const Q &rhs): Super (lhs, rhs)  {}
      
   template <class Scanner>
   typename P::value_type*  parse (Scanner &scanner) const
   {
      P::value_type*  func = left.parse (scanner);
      if (func)  return func;

      func = right.parse (scanner);
      if (!func)  return nullptr;

      scanner.getErrorInfo().clearRecord();
      return func;
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
   {
      //out << '(';
      writeOperator (out, " | ", level);
      //out << ')';
   }
};


} // nammespace _



template <class Signatur, bool = is_function<Signatur>::value>
class Operator: public BaseParser < _::operator_p<Signatur>>
{
   static_assert0 (function_traits<Signatur>::arity == 2 );   
public:
   
   template <class Func>
   Operator (const char *name, const Func& fun):  
      BaseParser < _::operator_p<Signatur>> (name, fun)  {}
};


template <class T>
class Operator <T, false>
   : public BaseParser < _::operator_p< T(T,T) >>
{
public:
   template <class Func>
   Operator (const char *name, const Func& fun):  
      BaseParser < _::operator_p< T(T,T) >> (name, fun)  {}
};



template <class Signatur, bool = is_function<Signatur>::value>
class UnaryOperator
   : public BaseParser <_::operator_p <Signatur, PID::UNARY_OPERATOR>>
{
   static_assert0 (function_traits<Signatur>::arity == 1 );   
public:
   template <class Func>
   UnaryOperator (const char *name, const Func& fun): Super (name, fun)  {}
};


template <class T>
class UnaryOperator <T, false>
   : public BaseParser <_::operator_p <T(T), PID::UNARY_OPERATOR>>
{
public:
   template <class Func>
   UnaryOperator (const char *name, const Func& fun): Super (name, fun)  {}
};



} // namespace pi

#endif
#ifndef _EXPRESSION_WRITER_H_
#define _EXPRESSION_WRITER_H_

#include "meta.h"


namespace PID { enum TYPE
{
   PRIMITIVE = 1000,
   RULE,
   LITERAL,
   NUMBER,
   EXPRESSION,
   FUNCTION,
   UNARY_OPERATOR,
   OPERATOR,
   

   UNARY_OPERATOR_CATEGORY = 2000,
   SUFFIX_INCREMENT,
   SUFFIX_DECREMENT,
   PREFIX_INCREMENT,
   PREFIX_DECREMENT,

   REPEAT,                 // unary -,+,*
   UMINUS,
   UPLUS,

   NOT,                    // !
   BITWISE_NOT,            // ~

   BINARY_OPERATOR_CATEGORY = 3000,
   LAZY_REPEAT,            // binary -,+,*


   MULTIPLY,
   DIVIDE,
   MODULO,
   LIST = MODULO,
   PLUS,
   MINUS,                  // -

   AND,                    // >>
   XOR,                    // ^ 
   OR,                     // |
   COMMA                   // ,
};}




enum class WRITEMODE: char { 
   MINIMAL,
   COMPACT,
   EXPAND,
   SURVEY,
};


template <class T>
std::string  toString (const T& expr, WRITEMODE mode = WRITEMODE::COMPACT)
{
   std::stringstream out;
   expr.toString (out, mode);
   return  out.str ();
}
   


template <class T>
struct  ExpressionWriter
{
   ExpressionWriter (const T& exp,  WRITEMODE lev): expression(exp), level(lev)  {}

   const T&  expression;
   WRITEMODE level;
};



template <class T>
ExpressionWriter<T>  printExpression (const T& data, WRITEMODE level = WRITEMODE::EXPAND)
{
   return  ExpressionWriter<T> (data, level);
}


template <class T>
std::ostream& operator<< (std::ostream &out, const ExpressionWriter<T> &helper)  
{ 
   helper.expression.toString (out, helper.level);
   return out; 
}




template <class P, class Q, int precedence = P::rank>
class BinaryExpression
{
public:
   enum {rank = precedence };

   typename ref_to<P>::type left;
   typename ref_to<Q>::type right;
protected:
   typedef BinaryExpression  Super;

   BinaryExpression (const P &lhs, const Q &rhs): left(lhs), right(rhs)  {}

   void writeOperator (std::ostream &out, const char *operator_name, WRITEMODE level) const
   {
      if (P::rank <= rank)
         left.toString (out, level);
      else 
      {
         out << '(';
         left.toString (out, level);
         out << ')';
      }

      out << operator_name;

      if (Q::rank <= rank)
         right.toString (out, level);
      else 
      {
         out << '(';
         right.toString (out, level);
         out << ')';
      }
   }

};


template <class P, int precedence = P::rank>
class UnaryExpression
{
public:
   enum { rank = precedence };

   typename ref_to <P>::type  child;
protected:
   typedef UnaryExpression  Super;


   UnaryExpression (const P& parser): child (parser)  {}

   void writeOperator (std::ostream &out, const char *op, WRITEMODE level) const
   {
      out << op;
      if (P::rank <= rank)
      {
         out << " ";
         child.toString (out, level);
      }
      else
      {
         out << '(';
         child.toString (out, level);
         out << ')';
      }
   }


   void writeOperatorInBrakets (std::ostream &out, const char *op, WRITEMODE level) const
   {
      out << op << '[';
      child.toString (out, level);
      out << ']';
   }

};


#endif


/*
   Operator
   2	++	Suffix increment	
      --	Suffix decrement	
      ()	Function call	
      []	Array subscripting	
      ->	Element selection through pointer	
   3	++	Prefix increment	            Right-to-left
      --	Prefix decrement	
      +	Unary plus	
      -	Unary minus	
      !	Logical NOT	
      ~	Bitwise NOT	
      *	Indirection (dereference)	
      &	Address-of	
   4	->*	Pointer to member (C++ only)		Left-to-right
   5	*	Multiplication	
      /	Division	
      %	Modulo (remainder)	
   6	+	Addition	
      -	Subtraction	
   7	<<	Bitwise left shift	
      >>	Bitwise right shift	
   8	<	Less than	
      <=	Less than or equal to	
      >	Greater than	
      >=	Greater than or equal to	
   9	==	Equal to	
      !=	Not equal to	
   10	&	Bitwise AND	
   11	^	Bitwise XOR (exclusive or)	
   12	|	Bitwise OR (inclusive or)	
   13	&&	Logical AND	
   14	||	Logical OR	
   15	=	Direct assignment		Right-to-left
      +=	Assignment by sum	
      -=	Assignment by difference	
      *=	Assignment by product	
      /=	Assignment by quotient	
      %=	Assignment by remainder	
      <<=	Assignment by bitwise left shift	
      >>=	Assignment by bitwise right shift	
      &=	Assignment by bitwise AND	
      ^=	Assignment by bitwise XOR	
      |=	Assignment by bitwise OR	
   17	,	Comma	Left-to-right
*/

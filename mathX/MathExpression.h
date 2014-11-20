#ifndef __MATHX_EXPRESSION_H
#define __MATHX_EXPRESSION_H


#include <ostream>
#include <Reference.h>
#include <Expressions.h>
#include <Allocator.h>
#include "NumberGenerator.h"
#include <meta/typetraits.h>


namespace mathX {





namespace Priority {
enum Level
{
   PRIMITIVE = 10000,
   INTEGER   = 11000,
   NUMBER    = 12000,
   VARIABLE  = 13000,

   GENERATOR = 20000,
   RANDOM_NUMBER,

   UNARY_FUNCTION,
   BINARY_FUNCTION,

   UNARY_OPERATOR = 30000,
   SUFFIX_INCREMENT,
   SUFFIX_DECREMENT,
   PREFIX_INCREMENT,
   PREFIX_DECREMENT,

   UMINUS,
   UPLUS,
   NOT,                    // !
   BITWISE_NOT,            // ~

   BINARY_OPERATOR = 31000,
   MULTIPLY,
   DIVIDE,
   MODULO,
   LIST = MODULO,
   PLUS,
   MINUS,                  // -

   RSHIFT,                 // >>
   XOR,                    // ^ 
   OR,                     // |
   COMMA                   // ,
};



bool LevelInCategory (Level level, Level category)
{
   int value = (int) level - (int) category;
   return  0 <= value && value < 1000;
}



Level  LevelToCategory (Level level)
{
   return  static_cast<Level> ((int) level / 1000);

}

} // namespace Priority



template <PID::TYPE op>
struct OperatorToken
{
   const char value =  
      op == PID::MINUS?   '-':
      op == PID::UMINUS?  '-':
      op == PID::PLUS?    '+':
      op == PID::UPLUS?   '+':
      op == PID::MULTIPLY? '*':
      op == PID::DIVIDE?  '/':
      op == PID::MODULO?  '%':
      op == PID::XOR?     '^':
      op == PID::EQUAL?   '=': '?';
};






class BaseExpression
{
public:
   const Priority::Level  priority;

   BaseExpression (Priority::Level level): priority (level)  {}
   virtual ~BaseExpression ()  {}


   virtual void prime (const NumberGenerator& gen)
   {
   }

   virtual double eval () const
   {
      return 0;
   }

   virtual void serialize (std::ostream& out) const = 0;
};





template <class Base>
struct make_expression: public BaseExpression
{
   using Super = make_expression;
   
   make_expression (Priority::Level priority): BaseExpression (priority)  {}

};



using  Allocator = mgo::Allocator <BaseExpression>;



//template <class T>
//BaseExpression*  makeExpression (const T& value);

BaseExpression*  makeExpression (int value);
BaseExpression*  makeExpression (double value);
BaseExpression*  makeExpression (const Range& range);
BaseExpression*  makeExpression (const std::initializer_list<int>& list);


class Expression
{
public:
   explicit Expression (Allocator& alloc): alloc (&alloc)  {}

   explicit Expression (Allocator& alloc, BaseExpression* ex)
      : expr(*ex),  alloc (&alloc)  
   {
      alloc.registerObject (ex);
   }


   void  assign (BaseExpression* ex)
   {
      expr.reassign ( *alloc->registerObject (ex));
   }



   template <class T>
   Expression&  operator= (T&& value)
   {
      assign (makeExpression (std::forward<T>(value)));
      return *this;
   }

   Expression&  operator= (const std::initializer_list<int>& list)
   {
      assign (makeExpression (Range{list}) );
      return *this;

   }

   BaseExpression& get ()
   {
      return expr;
   }

   const BaseExpression& get () const
   {
      return expr;
   }



   void prime (const NumberGenerator& gen)
   {
      expr->prime (gen);
   }

   double eval () const
   {
      return expr->eval ();
   }

   void serialize (std::ostream& out) const
   {
      expr->serialize (out);
   }



   Allocator& getAllocator () const
   {
      return *alloc;
   }

   static Allocator&  GlobalAllocator ()
   {
      static  Allocator allocator;
      return allocator;
   }


private:
   mgo::Reference <BaseExpression>  expr;
   Allocator   *alloc;
};




} // mathX
#endif 


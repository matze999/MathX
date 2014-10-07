#ifndef __MATH_EXPRESSION_H
#define __MATH_EXPRESSION_H


#include <ostream>
#include <Reference.h>
#include <Expressions.h>
#include <Allocator.h>



namespace math {





namespace Priority {
enum Level
{
   PRIMITIVE = 10000,
   INTEGER   = 11000,
   NUMBER    = 12000,
   VARIABLE  = 13000,

   GENERATOR = 20000,
   RANDOM_NUMBER,

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




class NumberGenerator
{
public:
   virtual void generate (int&) = 0;
   virtual void generate (double&) = 0;
};


class PlainWriter;



class BaseExpression
{
public:
   const Priority::Level  priority;

   BaseExpression (Priority::Level level): priority (level)  {}
   virtual ~BaseExpression ()  {}

   virtual void generate (NumberGenerator&)
   {
   }

   virtual double calculate ()
   {
      return 0;
   }

   virtual void serialize (PlainWriter&) = 0;
};





template <class Element, class Visitor>
class ElementVisitor
{
public:
   void execute (Element&, Visitor&)  {}
};



template <class Base>
struct make_expression: public BaseExpression
{
   using Super = make_expression;
   
   make_expression (Priority::Level priority): BaseExpression (priority)  {}

   virtual void serialize (PlainWriter& writer)
   {
      ElementVisitor <Base, PlainWriter>().execute (*this, writer);
   }
};





class Expression
{
   BaseExpression* data;
   Allocator*      allocator;

};


/*





Generators

binary_func : (Expr, Expr) -> Expr
unary_func  : Expr -> Expr

Number_gen : double -> Number
random_gen : #<word> -> RandomNumber

word -> variable

NumberAllocator num_alloc
Generator <Double, Expression>  number (num_alloc);




Generator <Expression>  number, random;
Function <Expression(Expression,Expression)>  plus, minus;

expression[alloc] = int_ 
                  | token(#) >> int_ >> '.' >> int_
                  | word;

plus[fn::plus] = expression




Device
Producer
Creator

number = _double;





*/


/*

struct Expression;



struct Function
{
   Function () = default;
   Function (const char* name): name (name)  {}

   const char* name;
   Expression *arg1, *arg2;
};



class Allocator
{
public:
   Function* newFunction ()
   {
      return  func_allocator.newObject ();
   }

   Function* newFunction (const char* name)
   {
      return  func_allocator.newObject (name);
   }



private:
   mgo::Allocator <Function>   func_allocator;
};



class BaseWriter
{
public:
   virtual  void serialize (std::ostream& out, double val) const
   {
      out << val;
   }

   virtual  void serialize (std::ostream& out, const char* str) const
   {
      out << str;
   }
};

class BaseGenerator;


struct ExpressionData
{
   ExpressionData (Priority::Level priority): priority(priority)  {}

   ExpressionData (Priority::Level priority, Allocator& alloc):
      priority(priority), allocator(alloc)  {}


   Priority::Level  priority;
   const char* name;
   
   union
   {
      int           integer;
      double        number;
      Function     *function;
   };

   mutable mgo::Reference <Allocator>  allocator;
};



class Expression: protected ExpressionData
{
public:
   Expression (Priority::Level priority) : ExpressionData(priority)  {}

   Expression (Priority::Level priority, Allocator& alloc, const char* name = nullptr)
      : ExpressionData(priority, alloc)  
   {
      this->name = name;
   }


   virtual double  eval() const
   {
      return number;
   }

   virtual void  generate (const BaseGenerator& gen)  {}

   virtual void  serialize (std::ostream& out, const BaseWriter& writer) const
   {
      writer.serialize (out, number);
   }


   operator  const ExpressionData&() const
   {
      return *this;
   }

   Allocator& getAllocator() const
   {
      return allocator;
   }
};



struct BinaryOperator: public Expression 
{
public:
   BinaryOperator (Priority::Level priority,  Allocator& alloc, const char* name):
      Expression (priority, alloc)
   {
      function = alloc.newFunction (name);
   }

   double  eval() const
   {
      switch (function->name[0])
      {
      case '+': 
         return  function->arg1->eval () + function->arg2->eval ();
      case '-': 
         return  function->arg1->eval () - function->arg2->eval ();
      case '*': 
         return  function->arg1->eval () * function->arg2->eval ();
      case '/': 
         return  function->arg1->eval () / function->arg2->eval ();
      }
   }


   template <class Generator>
   void generate (const Generator& gen)
   {
      function->arg1->generate (gen);
      function->arg1->generate (gen);
   }

   void  serialize (std::ostream& out, const BaseWriter& writer) const
   {
      writer.serializeFunction (out, number);
   }


private:
   
};




template <class P, class Q>
using Plus = BinaryOperator <fn::Plus, P, Q, PID::PLUS>;

template <class P, class Q>
using Minus = BinaryOperator <fn::Minus, P, Q, PID::MINUS>;

template <class P, class Q>
using Multiply = BinaryOperator <fn::Multiply, P, Q, PID::MULTIPLY>;

template <class P, class Q>
using Divide = BinaryOperator <fn::Divide, P, Q, PID::DIVIDE>;

//template <class P, class Q>
//using Exponent = BinaryOperator <fn::Plus, P, Q>;

*/


} // math
#endif 


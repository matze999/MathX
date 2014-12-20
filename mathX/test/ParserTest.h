#ifndef __MATHX_TEST_PARSER_H_
#define __MATHX_TEST_PARSER_H_


#include <gtest++.h>
#include "../NumberGenerator.h"
#include "../Number.h"
#include "../ExpressionParser.h"

using namespace mathX;
using namespace pi;



struct ExpressionParserTest: public testing::Test
{
   ExpressionParserTest (): parser (alloc)
   {}

   
   virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
   }

   virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
   }

   void assertRandomNumber (Expression& expr)
   {
      double res = expr.eval ();
      expr.prime (gen);
      ASSERT_NE (res, expr.eval());
   }

   double  parseRandomNumber (const char* input);

   Allocator alloc;
   NumberGenerator gen;

   ExpressionParser  parser;
};


TEST_F (ExpressionParserTest, testIntegerInput)
{
   Expression  number (alloc);


   parser.parse ("12", number);
   
   number.prime (gen);
   ASSERT_EQ (12, number.eval ());

   parser.parse ("// hallo \n 4\t", number);
   number.prime (gen);
   ASSERT_EQ (4, number.eval ());
}



double  ExpressionParserTest::parseRandomNumber (const char* input)
{
   Expression  number (alloc);
   std::stringstream out;

   double result;
   try
   {
      parser.parse (input, number);
      number.prime (gen);
      number.serialize (out);
      out >> result;
   }
   catch (ErrorInfo& info)
   {
      cout << info;
      return 0;
   }
   return  result;
}



TEST_F (ExpressionParserTest, testRandomNumber)
{
   double result;
   REPEAT (10)
   {
      result = parseRandomNumber ("#1");
      ASSERT_IN (result, -10, 10);
      ASSERT_TRUE (isInteger (result));


      result = parseRandomNumber ("#1.1");
      ASSERT_IN (result, -1, 1);


      result = parseRandomNumber ("#[1,4]");
      ASSERT_IN (result, 1, 4);
      ASSERT_TRUE (isInteger (result));



      //TRC_LEVEL = TL_DEBUG;
      result = parseRandomNumber ("#*1");
      ASSERT_NE (0, result) << "zero generated for #*1";


      result = parseRandomNumber ("#*1.1");
      ASSERT_NE (0, result) << "zero generated for #*1.1";

   }
}



TEST_F(ExpressionParserTest, testSum)
{
   Expression expression(alloc);

   parse(parser, "4+3", expression);
   ASSERT_EQ (7, expression.eval());


   parse(parser, "1-2", expression);
   ASSERT_EQ (-1, expression.eval());

   parse(parser, "1 + 1 + 1", expression);
   ASSERT_EQ (3, expression.eval());

   parse(parser, "1 - 1 + 1", expression);
   ASSERT_EQ (1, expression.eval());
}


TEST_F(ExpressionParserTest, testProduct)
{
   Expression expression(alloc);

   parse(parser, "4*3", expression);
   ASSERT_EQ(12, expression.eval());


   parse(parser, "1/2", expression);
   ASSERT_EQ(0.5, expression.eval());

   parse(parser, "1 * 1 * 1", expression);
   ASSERT_EQ(1, expression.eval());

   parse(parser, "2 / 1 * 3", expression);
   ASSERT_EQ(6, expression.eval());
}


#endif
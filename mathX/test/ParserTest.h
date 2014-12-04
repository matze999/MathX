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


TEST_F (ExpressionParserTest, testRandomNumber)
{
   Expression  number (alloc);

   TRC_LEVEL = TL_DEBUG;
   try
   {
      parser.parse ("#1", number);
   }
   catch (ErrorInfo& info)
   {
      cout << info;
      ASSERT_TRUE (false);
   }

   REPEAT (10)
   {
      number.prime (gen);
      ASSERT_IN (number.eval(), 0, 10);
   }
}


#endif
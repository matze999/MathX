#ifndef __MATHX_TEST_VARIABLE_H_
#define __MATHX_TEST_VARIABLE_H_


#include <random>
#include <sstream>
#include <gtest++.h>
#include "../Variable.h"
#include <cmath>
#include <pi/parser.h>


using namespace mathX;




struct VariableTest: public testing::Test
{
   VariableTest(): parser(alloc)
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



TEST_F (VariableTest, initialize)
{
   Variable var1 {"x"}, var2;

   ASSERT_TRUE(std::isnan(var1.eval()));

   stringstream out;
   var1.serialize(out);
   ASSERT_EQ("x", out.str());

   out.str("");
   var2.serialize(out);
   ASSERT_EQ("var", out.str());
}


TEST_F (VariableTest, assignExpression)
{
   Expression  var1(alloc), var2(alloc);

   var1 = "x";
   stringstream out;
   ASSERT_TRUE (var1.isInitialized());

   var1.serialize(out);
   ASSERT_EQ ("x", out.str());

   var2 = "x x";
   out.str("");
   var2.serialize(out);
   ASSERT_EQ("x x", out.str());

}


TEST_F (VariableTest, parseAndSerialize)
{
   using namespace pi;
   Expression  var(alloc);

   parse(parser, "x", var);
   ASSERT_TRUE (var.isInitialized());

   stringstream out;
   var.serialize(out);
   ASSERT_EQ ("x", out.str());


   parse(parser, "x*3", var);
   ASSERT_TRUE (var.isInitialized());

   out.str ("");
   var.serialize(out);
   ASSERT_EQ ("x*3", out.str());
   ASSERT_TRUE (isnan(var.eval()));

}


#endif
#ifndef __MATHX_TEST_PRIMITIVE_H_
#define __MATHX_TEST_PRIMITIVE_H_


#include <random>
#include <sstream>
#include <gtest++.h>
#include "../Number.h"
#include "../NumberGenerator.h"


using namespace mathX;







TEST (PrimitiveTest, IntegerTest)
{
   NumberGenerator gen;

   int expected_result =  gen();
   Number<int> value = expected_result;
   ASSERT_EQ (expected_result, value.eval());

   value = 4;
   ASSERT_EQ (4, value.eval());

   value.prime (gen);
   ASSERT_EQ (4, value.eval());

   std::stringstream out;
   value.serialize (out);
   ASSERT_EQ ("4", out.str());
}




TEST (PrimitiveTest, RandomIntTest)
{

   Random<int>  number;
   NumberGenerator gen;
   number.prime (gen);

   double result = number.eval();

   number.prime (gen);
   ASSERT_NE (result,  number.eval ());

   result = number.eval ();
   ASSERT_TRUE (isInteger (result));
}



TEST (PrimitiveTest, BoundRandomIntTest)
{

   Random<int>  number{ 0, 1 };
   NumberGenerator gen;

   for (int loop = 1; loop < 100; ++loop)
   {
      number.prime (gen);

      double result = number.eval ();

      ASSERT_LE (result, 1);
      ASSERT_GE (result, 0);
   }


   RandomNot0<int>  number0;
   number0 = { -1, 1 };
   for (int loop = 1; loop < 100; ++loop)
   {
      number0.prime (gen);
      ASSERT_NE (0, number0.eval());
   }
}



TEST (PrimitiveTest, DoubleTest)
{
   NumberGenerator  gen;
   double  expected_result;
   gen (expected_result);


   Number<double>  value {expected_result};
   ASSERT_EQ (expected_result, value.eval());

   value = 4.123;
   ASSERT_EQ (4.123, value.eval());

   value.prime (gen);
   ASSERT_EQ (4.123, value.eval());

   std::stringstream out;
   value.serialize (out);
   ASSERT_EQ ("4.123", out.str());
}



TEST (PrimitiveTest, RandomDoubleTest)
{

   Random<double>  number;
   NumberGenerator gen;
   number.prime (gen);

   double val1 = number.eval();
   number.prime (gen);
   double val2 = number.eval();

   ASSERT_NE (val1,  val2);
   ASSERT_FALSE (isInteger (val2));
}



TEST (PrimitiveTest, BoundRandomDoubleTest)
{
   Random<double>  number (1,1);
   NumberGenerator gen;

   for (int loop = 1; loop < 100; ++loop)
   {
      number.prime (gen);
      double val1 = number.eval ();

      ASSERT_IN (val1, -1, 1);
   }

   RandomNot0 <double> number0 (1, 1);
   for (int loop = 1; loop < 100; ++loop)
   {
      number0.prime (gen);
      ASSERT_NE (0, number.eval());
   }
}


#endif


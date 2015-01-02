#ifndef __MATHX_TEST_OPERATOR_H_
#define __MATHX_TEST_OPERATOR_H_


#include <sstream>
#include <gtest++.h>
#include "../NumberGenerator.h"
#include "../Number.h"
#include "../Operator.h"

using namespace mathX;




bool  is_equal (double v1, double v2)
{
   return  abs(v1 - v2) < 0.00000001;
}


bool is_integer (double value)
{
   return  abs(value - int(value)) < 0.00000001;
}



TEST (OperatorTest, ExpressionAssignment)
{
   Allocator  alloc;
   Expression  number (alloc);
   NumberGenerator gen;

   number = { 0, 1 };

   number.prime (gen);
   ASSERT_IN (number.eval(), 0, 1);


   number = Intval (4, 5);
   number.prime (gen);
   ASSERT_IN (number.eval(), 4, 5);

   REPEAT (10)
   {

      number = Accuracy (1, 0);
      number.prime (gen);
      double  value = number.eval();

      ASSERT_IN (value, -10, 10);
      ASSERT_EQ (value, (int) value);
   }


   REPEAT (10)
   {
      number = Accuracy (4, 2);
      number.prime (gen);
      double  value = number.eval();
      value *= 100;
      ASSERT_INT (value);

      value /= 10000;
      ASSERT_IN (value, -1, 1);
   }
}


TEST (OperatorTest, Plus)
{
   Allocator alloc;
   Expression v1 (alloc), v2 (alloc);

   v1 = 1;
   v2 = 1;

   Expression plus = v1 + v2;
   ASSERT_EQ (2, plus.eval());


   v2 = 1.2;
   ASSERT_DOUBLE_EQ (0.2, (v2-v1).eval());
}



#endif
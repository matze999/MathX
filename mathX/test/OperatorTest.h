#ifndef __MATHX_TEST_OPERATOR_H_
#define __MATHX_TEST_OPERATOR_H_


#include <sstream>
#include <gtest/gtest.h>
#include "../NumberGenerator.h"
#include "../Number.h"
#include "../Operator.h"

using namespace mathX;



bool  is_equal (double v1, double v2)
{
   return  (v1 - v2) < 0.00000001;
}

TEST (OperatorTest, ExpressionAssignment)
{
   Allocator  alloc;
   Expression  number (alloc);
   NumberGenerator gen;

   number = {0, 1};

   number.prime (gen);
   ASSERT_GE (number.eval(), 0);
   ASSERT_LE (number.eval(), 2);
}


TEST (OperatorTest, Plus)
{

   //BinaryFunction  result2 = val1 + val2;

   //Pool pool;
   //Expression v1(pool), v2(pool);


   Allocator alloc;
   Expression v1 (alloc), v2 (alloc);

   v1 = 1;
   v2 = 1;

   Expression plus = v1 + v2;
   ASSERT_EQ (2, plus.eval());


   v2 = 1.2;
   ASSERT_TRUE ( is_equal(0.2, (v2-v1).eval()) );

   
}

#endif
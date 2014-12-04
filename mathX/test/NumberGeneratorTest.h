#ifndef __MATHX_TEST_NUMBER_GENERATOR_H_
#define __MATHX_TEST_NUMBER_GENERATOR_H_


#include <sstream>
#include <gtest/gtest.h>
#include "../NumberGenerator.h"


using namespace mathX;





TEST (NumberGeneratorTest, IntegerGeneration)
{
   for (unsigned loop = 1; loop < 100; ++loop)
   {
      NumberGenerator gen1;
      Intval range{ 0, 1 };
      int value = gen1(range);
      ASSERT_LE (value, 1);
      ASSERT_GE (value, 0);

      NumberGenerator gen2;
      Intval range2{ -13, -8 };
      value = gen2 (range2);
      ASSERT_LE (value, -8);
      ASSERT_GE (value, -13);
   }
}




//   std::cout << "lb: " << lb << std::endl;
//   std::cout << "ub: " << ub << std::endl;



#endif
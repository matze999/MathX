
#include <gtest/gtest.h>
#include "NumberGeneratorTest.h"
#include "PrimiteTest.h"
#include "OperatorTest.h"
#include "ParserTest.h"

#include "VariableTest.h"
//#include <iostream>


using namespace std;





int main (int argc, char** argv)
{

   testing::InitGoogleTest (&argc, argv);

   RUN_ALL_TESTS (); 

   getchar ();
   return 0;
}




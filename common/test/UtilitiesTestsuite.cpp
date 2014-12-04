#include <iostream>
using namespace std;

#include <gtest/gtest.h>
#include "InlistTest.h"
#include "MetaProgrammingTest.h"
#include "StringHelperTest.h"


#include "wrapper_test.h"
#include "test_SparseVector.h"
//#include "test_Reference.h"


int main (int argc, char** argv)
{
   //TRC_LEVEL = TL_DEBUG;
   testing::InitGoogleTest (&argc, argv);
   RUN_ALL_TESTS (); 

   getchar ();
   return 0;

}


#include <iostream>
using namespace std;

#include "wrapper_test.h"
#include "test_SparseVector.h"
#include "test_Reference.h"


int main()
{
   wrapper_test();
   test_SparseVector ();
   test_Reference ();

   cout << "All tests are OK!" << endl;
   getchar();
   return 0;
}


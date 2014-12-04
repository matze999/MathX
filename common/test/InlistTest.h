#ifndef _MGO_TEST_INLIST_H_
#define _MGO_TEST_INLIST_H_



#include "../gtest++.h"
#include "../Inlist.h"




TEST (InlistTest, testIntInterval)
{
   int a = 4;

   ASSERT_TRUE (a IN (4));
   ASSERT_TRUE (0 IN (4));
   ASSERT_FALSE (-1 IN (9));

   ASSERT_TRUE (a IN (4, 9));
   ASSERT_FALSE (a IN (5, 9));

   double d = 5;
   ASSERT_TRUE (d IN (4, 9));

}



TEST (InlistTest, testCharInterval)
{
   char a = '4';

   ASSERT_TRUE (a IN ('0', '9'));
   ASSERT_FALSE (a IN ('5', '9'));

   ASSERT_TRUE (a NOT_IN ('5', '9'));
}



TEST (InlistTest, testInitializerList)
{
   int a = 4, b = 6;

   ASSERT_TRUE ((a IN { 2, 4, 6 }));

   ASSERT_TRUE ((a IN { 2, 4, b, -1 }));

   ASSERT_FALSE ((a IN { 2, 5, 7, 9, 11 }));

   double d = 4;
   ASSERT_TRUE ((d IN { 2, 4, 6 }));
}


#endif


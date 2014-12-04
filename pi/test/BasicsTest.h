#ifndef  __TEST_PARSER_BASICS_
#define  __TEST_PARSER_BASICS_


#include <gtest++.h>
#include <pi/parser.h>


using namespace pi;


TEST (BasicsTest, IsScanner)
{
   ASSERT_TRUE (pi::mp::is_scanner <Scanner<>>::value);
   ASSERT_TRUE (pi::mp::is_scanner <SkipScanner<WhiteSpace>>::value);
   ASSERT_TRUE (pi::mp::is_scanner <typename Scanner<>::UpperCaseScanner>::value);


   ASSERT_FALSE (pi::mp::is_scanner <int>::value);
   ASSERT_FALSE (pi::mp::is_scanner <WhiteSpace>::value);
}


#endif


#ifndef  _GTEST_PP_H_
#define  _GTEST_PP_H_


#include <gtest/gtest.h>



#define REPEAT__(loops, line)  \
   for (int _loop##line = 0; _loop##line < loops; ++_loop##line)

#define REPEAT_(loops, line)   REPEAT__ (loops, line)
#define REPEAT(loops)   REPEAT_ (loops, __LINE__)



#define ASSERT_IN(val, lb, ub)       ASSERT_GE (val, lb); \
                                     ASSERT_LE (val, ub);



#define NOTEST(a, b)  void a##b()
#define NOTEST_F(a, b)  void a##b()

#endif

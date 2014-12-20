#ifndef __MATHX_NUMBER_GENERATOR_H
#define __MATHX_NUMBER_GENERATOR_H


#include <random>
#include <cmath>
//#include <IEEE754.h>
#include <util.h>

namespace mathX {


struct Intval
{
   static const int MIN = 0x80000000, MAX = 0x7fffffff;

   Intval (int min, int max): lb (min), ub (max)  {}

   Intval () = default;

   Intval (std::initializer_list<int> list)
   {
      assign (list);
   }


   Intval& operator= (std::initializer_list<int> list)
   {
      assign (list);
      return *this;
   }

   int  width () const
   {
      return ub - lb + 1;
   }

   int lb, ub;

private:
   void assign (const std::initializer_list<int>& list)
   {
      assert (list.size() == 2);
      auto pos = list.begin ();
      lb = *pos;
      ub = *++pos;

      assert (lb < ub);
   }
};



const bool NOT_ZERO = true;


class NumberGenerator
{
public:

   NumberGenerator ()  {}


   int  operator() (const Intval&  range) const;

   int  operator() () const
   {
      return  (int) device();
   }


   int  operator() (bool exclude_zero) const
   {
      int value = operator()();
      if (exclude_zero) while (value == 0)
      {
         value = operator()();
      }
      return value;
   }


   int  operator() (const Intval&  range, bool exclude_zero) const
   {
      int value = operator()(range);
      if (exclude_zero) while (value == 0)
      {
         value = operator()(range);
      }
      return value;
   }

   void  operator() (double& value) const;


private:
   mutable std::random_device device;
};



struct Accuracy
{
   Accuracy (): mantisse (6), decimals (0), exclude_zero (false)  {}

   Accuracy (int digits, int decimals): mantisse (digits), decimals (decimals), exclude_zero (false)  {}

   double  generate (const NumberGenerator& gen);

   int mantisse;
   int decimals;
   bool exclude_zero;
};




} // namespace mathX
#endif 


#ifndef __MATHX_NUMBER_GENERATOR_H
#define __MATHX_NUMBER_GENERATOR_H


#include <random>
#include <cmath>
#include <IEEE754.h>
#include <util.h>

namespace mathX {



struct Range
{
   static const int MIN = 0x80000000, MAX = 0x7fffffff;

   Range (int min, int max): lb (min), ub (max)  {}

   Range (): Range (MIN, MAX)  {}

   Range (std::initializer_list<int> list)
   {
      assign (list);
   }


   Range& operator= (std::initializer_list<int> list)
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


   int  operator() (const Range&  range) const;

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
   }

   void  operator() (double& value) const;


private:
   mutable std::random_device device;
};



struct Accuracy
{
   static const unsigned UNBOUNDED = -1;

   Accuracy (): mantisse (10), decimals (UNBOUNDED)  {}

   Accuracy (int digits, int decimals): mantisse (digits), decimals (decimals)  {}

   double  generate (const NumberGenerator& gen);

   unsigned mantisse;
   unsigned decimals;
};





template <class Generator, class Constraint>
class ConstraintedNumberGenerator
{
public:
   int generateInteger ()
   {
      return  constr (generator.generateInteger ());
   }


   double generateDouble ()
   {
      return  constr (generator.generateDouble ());
   }

private:
   Generator generator;
   Constraint constr;
};



class RangeConstraint
{
public:
   RangeConstraint (int lb, int ub): lower_bound (lb), range_width (ub-lb)  {}

   int operator() (int value)
   {
      return  lower_bound + value % range_width;
   }

   double operator() (double value)
   {
      while (value < lower_bound)
         value += range_width;

      while (value > lower_bound + range_width)
         value -= range_width;

      return  value;
   }

private:
   int lower_bound, range_width;
};



} // namespace mathX
#endif 


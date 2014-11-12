#ifndef __MATHX_NUMBER_H
#define __MATHX_NUMBER_H

#include "NumberGenerator.h"

namespace mathX {




template <class T>
struct Number
{
   Number ()  {}//: Super (Priority::NUMBER)  {}

   Number (T val)
   {
      value = val;
   }

   double eval () const
   {
      return value;
   }

   void prime (const NumberGenerator&  gen)  {}



   void serialize (std::ostream& out) const
   {
      out << value;
   }

   T value;
};


using
Integer = Number <int>;

using
Double = Number <double>;




template <class T> class Random;

template <>
struct Random<Integer>: public Integer
{
   Random (): range (-1000, 1000)  {}
   Random (int lb, int ub): range (lb, ub)  {}


   void  bound (int lb, int ub)
   {
      range = {lb, ub};
   }

   void prime (const NumberGenerator&  gen)  
   {
      value = gen (range);
   }

private:
   Range  range;
};




template <>
struct Random<Double>: public Double
{
   Random (): accuracy (6, 2)  {}

   Random (int mantisse, int decimals): accuracy (mantisse, decimals)  {}


   void  setAccurancy (int mantisse, int decimals)
   {
      accuracy.mantisse = mantisse;
      accuracy.decimals = decimals;
   }


   void prime (const NumberGenerator&  gen)  
   {
      value = accuracy.generate (gen);

   }
private:
   Accuracy accuracy;
};



template <class T>
struct RandomNot0: public Random<T>
{
   template <class ...Args>
   RandomNot0 (Args&&... args): Random<T> (std::forward<Args>(args)...)  {}

   void prime (const NumberGenerator&  gen)
   {
      do { 
         Random<T>::prime (gen);
      } while (value == 0);
   }
};
 
} // namespace mathX
#endif 


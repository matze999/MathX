#ifndef __MATHX_NUMBER_H
#define __MATHX_NUMBER_H

#include "MathExpression.h"

namespace mathX {




template <class T>
struct Number: public make_expression <Number<T>>
{
   Number (): Super (Priority::NUMBER)  {}

   Number (T val): Super (Priority::NUMBER)  
   {
      value = val;
   }

   Number&  operator= (T val)
   {
      value = val;
      return *this;
   }

   double eval () const
   {
      return value;
   }


   void serialize (std::ostream& out) const
   {
      out << value;
   }

   T value;
};



template <class T> class Random;

template <>
struct Random<int>: public Number<int>
{
   Random (): range (-1000, 1000)  {}
   Random (const Intval& range): range (range)  {}
   Random (int lb, int ub): range (lb, ub)  {}
   Random (const std::initializer_list<int>&  list): range (list)  {}


   Random&  operator= (const std::initializer_list<int>&  list)
   {
      range = list;
      return *this;
   }

   void prime (const NumberGenerator&  gen)  
   {
      value = gen (range);
   }

private:
   Intval  range;
};




template <>
struct Random<double>: public Number<double>
{
   Random (): accuracy (5, 2)  {}

   Random (int mantisse, int decimals): accuracy (mantisse, decimals)  {}

   Random (const Accuracy& accuracy): accuracy (accuracy)  {}


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
class RandomNot0: public Random<T>
{
public:
   template <class ...Args>
   RandomNot0 (Args&&... args): Random<T> (std::forward<Args>(args)...)  {}


   RandomNot0&  operator= (const std::initializer_list<int>& list)
   {
     Random<T>::operator= (list);
      return *this;
   }


   void prime (const NumberGenerator&  gen)
   {
      do { 
         Random<T>::prime (gen);
      } while (value == 0);
   }
};
 



inline
BaseExpression*  makeExpression (int value)
{
   return  new Number<int> (value);
}

inline
BaseExpression*  makeExpression (double value)
{
   return  new Number<double> (value);
}


inline
BaseExpression*  makeExpression (const Intval& intval)
{
   return  new Random <int> {intval};
}

inline
BaseExpression*  makeExpression (const std::initializer_list<int>& list)
{
   return  new Random <int> {list};
}

inline
BaseExpression*  makeExpression (const Accuracy& accuracy)
{
   assert0 (accuracy.mantisse < 10);
   if (accuracy.decimals == 0)
   {
      int limit = (int) std::pow (10, accuracy.mantisse);
      if (accuracy.exclude_zero)
         return  new RandomNot0 <int> (-limit, limit);
      else
         return  new Random <int> (-limit, limit);
   }
   return  new Random <double> (accuracy);
}



} // namespace mathX
#endif 


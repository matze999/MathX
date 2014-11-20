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
   Random (const Range& range): range (range)  {}
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
   Range  range;
};




template <>
struct Random<double>: public Number<double>
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
BaseExpression*  makeExpression (const Range& range)
{
   return  new Random <int> {range};
}

inline
BaseExpression*  makeExpression (const std::initializer_list<int>& list)
{
   return  new Random <int> {list};
}




} // namespace mathX
#endif 


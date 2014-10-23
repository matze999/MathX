
#include "expression/Operator.h"


namespace pi {

template <class T>
struct  op
{
   static Operator<T>  plus, minus, multiply, divide;
};



template <class T>
Operator<T>  op<T>::plus = { "+", std::plus < T > {} };

template <class T>
Operator<T>  op<T>::minus = { "-", std::minus < T > {} };

template <class T>
Operator<T>  op<T>::multiply = { "*", std::multiplies < T > {} };

template <class T>
Operator<T>  op<T>::divide = { "/", std::divides < T > {} };



} // namespace pi

#ifndef __HAS_FUNCTION_CALL_H_
#define __HAS_FUNCTION_CALL_H_

#include "typetraits.h"

namespace  mp {


template <class T>
T&  Instance ();


   
template <class A1 = void, class A2 = void, class A3 = void>
struct  has_function_base
{
   template <class T>
   static decltype (Instance<T>() (Instance<A1>(), Instance<A2>(), Instance<A3>()) )
      getType (T&);
};


template <class A1, class A2>
struct  has_function_base <A1, A2>
{
   template <class T>
   static decltype (Instance<T>() (Instance<A1>(), Instance<A2>()) )  getType (T&);
};


template <class A1>
struct  has_function_base <A1>
{
   template <class T>
   static decltype (Instance<T>() (Instance<A1>()) )  getType (T&);
};


template <>
struct  has_function_base <>
{
   template <class T>
   static decltype (Instance<T>() ()) getType (T&);
};



template <class T, class A1 = void, class A2 = void, class A3 = void>
class has_function_call: private has_function_base <A1, A2, A3>
{
   using  has_function_base <A1, A2, A3>::getType;
   static any_type getType (...);

public:
   typedef  decltype (getType (Instance<T>()) )  type;

   static const bool value = !is_same <any_type, type>::value;
};




} // namespace mp
#endif

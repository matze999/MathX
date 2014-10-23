#ifndef __VARIETY_LIST_H_
#define __VARIETY_LIST_H_

#include "typetraits.h"

namespace  mp {



template <class T = void, class ...Args>
struct variadic_size
{
   static const int value = variadic_size<Args...>::value + 1;
};

template <>
struct variadic_size<>
{
   static const int value = 0;
};



template <class T1 = void, class T2 = void, class ...Args>
struct variety_list
{
   typedef  variety_list <T2, Args...>  tail_type;
   T1  _head;
   tail_type _tail;
public:
   static const int size = variadic_size<Args...>::value + 2;

   variety_list (const T1& val1, const T2& val2, const Args&... args)
      : _head (val1), _tail (val2, args...)  {}

         T1&  head ()               { return _head; }
   const T1&  head () const         { return _head; }

         tail_type&  tail ()        { return _tail; }
   const tail_type&  tail () const  { return _tail; }

};

template <>
struct variety_list<>
{
   //any_type  head () const            { return any_type(); }

   //const variety_list&  tail () const  { return *this; }
};



template <class T1>
struct variety_list <T1>
{
   T1  _head;
public:
   static const int size = 1;

   variety_list (const T1& val1): _head (val1)  {}

         T1&  head ()               { return _head; }
   const T1&  head () const         { return _head; }

   variety_list<>  tail () const    { return variety_list<>(); }
};


template <class T1>
using unary_list = variety_list<T1>;

using empty_list = variety_list<>;



template <class ...Args>
variety_list <Args...> vlist (Args&&... args)
{
   return  variety_list <Args...> (args...);
}




} // namespace mp
#endif

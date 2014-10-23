#ifndef __TYPETRAITS_
#define __TYPETRAITS_

#include "if.h"
#include <type_traits>
#include <boost/type_traits/function_traits.hpp>



template <class Base>
struct ref_to: public std::tr1::remove_const <Base>  {};



namespace mp {

using namespace std;



struct any_type
{
   //any_type () = default;

   template <class T>
   any_type (T&) {}
};


template <class T>
T&  Instance ();



//*** lazy enable if

template <bool cond, class T>
struct lazy_enable_if
{
   typedef typename T::type type;
};

template <class T>
struct lazy_enable_if <false, T> {};


template <bool cond, class T = void>
using  lazy_enable_if_t = typename lazy_enable_if <cond, T>::type;


template <class cond, class T = void>
using  enable_cif_t = typename enable_if <cond::value, T>::type;

template <class cond, class T = void>
using  lazy_enable_cif_t = typename lazy_enable_if <cond::value, T>::type;



template <template <class> class U, class V>
class is_template_of
{
   template <class T>
   static  char check (const U<T>&);
   static  int  check (...);
   static  V&   instance();
public:
   static const bool value = sizeof check(instance()) == 1;
};



template <class T>
struct is_function_pointer: public
   and <is_pointer <T>, 
        is_function <typename remove_pointer<T>::type>>  {};


//***  function_traits <function>

// premise:  function template argument is a function type, e.g.  resT (arg1, arg2)
//class function_tag {};
class functor_tag {};


template <typename function> 
struct function_traits: public boost::function_traits <function>,
                        public functor_tag
{
   typedef  function_traits  signatur;
};


template <typename T>
struct is_functor: public is_base_of <functor_tag, T>  {};



template <typename T>
struct remove_cr: public remove_const <typename remove_reference <T>::type>  {};



template <typename T>
struct make_const: public add_const <T>  {};


template <typename T>
struct make_const<T&> 
{
   typedef  typename add_const<T>::type&  type;
};



template <class U, class V>
class is_like
{
   static  V Instance();

   static  char check (U);
   static  int  check (...);
public:
   static  const bool value = (sizeof check (Instance ()) == 1);
};



template <class U, class V>
struct promote_const  
{
   typedef V type;
};

template <class U, class V>
struct promote_const <const U, V>: public make_const<V> {};



template <class P, class Q>
struct lazy_replace_void
{
   typedef P  type;
};


template <class Q>
struct lazy_replace_void <void, Q>
{
   typedef typename  Q::type  type;
};



} // namespace mp
#endif // __META_


/// common type

//template <class A, class B>
//struct common_type:  
//   public common_type <typename remove_cr<B>::type, typename remove_cr<A>::type>  {};
//
//
//template <class A>
//struct common_type <A, A>: public promote <A>  {};
//
//template <>
//struct common_type <int, double>: public promote <double>  {};
//
//template <>
//struct common_type <float, double>: public promote <double>  {};
//


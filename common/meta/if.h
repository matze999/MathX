#ifndef __META_IF__
#define __META_IF__

#include <xtr1common>


/*** 

>>> IF example

IF value == end THEN
   typeA
ELSE 
   IF value == -1 THEN
      typeB
   ELSE
      typeC
   END
END::type

or
if_ <value = end, typeA,
if_ <value == -1, typeB,
                  typeC >>::type


>>> class and and or

if (and <A, B, C>::value || A::value || B::value)
{
}

*/



//#define  IF           ::mp::if_ <
//#define  THEN         ,
//#define  ELSE         ,
//#define  END          > 

#define  else_        

namespace mp {


template <class T>
struct promote 
{
   typedef  T  type;
};



template <bool cond, class T1, class T2>
struct if_  
{ 
   typedef T1 type; 
};

template <class T1, class T2>
struct if_ <false, T1, T2>
{ 
   typedef T2 type; 
};


template <class T1, bool cond2, class T2, class T3>
struct if_ <false, T1, if_ <cond2, T2, T3>>: public if_ <cond2, T2, T3>  {};


template <class T>
struct not: public std::integral_constant <bool, !T::value>  {};


template <class T1, class T2, class T3 = true_type, class T4 = true_type>
struct and: public 
   std::integral_constant <bool, T1::value && T2::value && T3::value && T4::value>  {};


template <class T1, class T2, class T3 = false_type, class T4 = false_type>
struct or: public 
   std::integral_constant <bool, T1::value || T2::value || T3::value || T4::value>  {};





} // namespace mp
#endif



/* andere imp. Moeglichkeit


template <bool cond>
struct _IF
{
   template <class trueT>
   struct _THEN
   {
      template <class elseT>
      struct _ELSE 
      {
         typedef trueT  _RET;
      };
   };
};


template <>
struct _IF <false>
{
   template <class trueT>
   struct _THEN
   {
      template <class elseT>
      struct _ELSE 
      {
         typedef elseT  _RET;
      };
   };
};


template <class T1, class T2>
struct _IF_EQUAL2
{
   template <class trueT>
   struct _THEN
   {
      template <class elseT>
      struct _ELSE 
      {
         typedef elseT  _RET;
      };
   };
};

template <class T>
struct _IF_EQUAL <T,T>
{
   template <class trueT>
   struct _THEN
   {
      template <class elseT>
      struct _ELSE 
      {
         typedef trueT  _RET;
      };
   };
};

*/

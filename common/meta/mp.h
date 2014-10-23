#ifndef __SMP_
#define __SMP_



BEGIN (namespace mgo)


const int ALL = ~0u >> 1;
const int NAN = ~ALL;



template <bool cond, typename THEN, typename ELSE>
struct IF
{
   typedef  THEN  RET;  
};

template <typename THEN, typename ELSE>
struct IF
{
   typedef  ELSE  RET;  
};



template <int val, typename T>
struct ILIST
{
   enum { HEAD = val };
   typedef  T  TAIL;
};

struct NIL
{
   enum { HEAD = NAN };
   typedef  NIL  TAIL;
};


template <typename LIST>
struct SIZE
{
   enum { VAL =  HEAD == NAN? 0: 1 + SIZE <LIST::TAIL> ::VAL; };
};

template <>
struct SIZE <void>
{
   enum { VAL = 0; };
};



template <typename LIST, int n>
struct AT
{
   enum { VAL = n == 0?
                LIST::HEAD:
                AT <LIST::TAIL, n-1> ::VAL };

   template <typename baseT>
   static
   void  ASSIGN (baseT *array, int val)
   {
      array [ind] = val;
   }
};

template <int n>
struct AT <void, n>
{
   enum { VAL = NAN };

   template <typename baseT>
   static
   void  ASSIGN (baseT *array, int val)  {}
};



template <typename LIST>
struct SUM
{
   enum { VAL =  LIST::HEAD + SUM <LIST::TAIL> ::VAL };
};

template <>
struct SUM <void>
{
   enum { VAL = 0 };
};


template <typename T>
struct FW
{
   typedef T  RET;
};



template <int v1 = NAN, int v2 = NAN, int v3 = NAN, int v4 = NAN>
struct LIST
{
   typedef  ILIST <v1, LIST <v2, v3, v4, NAN> ::RET>  RET;
};

template <int val>
struct LIST <val, NAN, NAN, NAN>
{
   typedef  IF <val == NAN, 
               void, 
               ILIST <val, void>
            >::RET  RET;
};



template <typename LIST, int value, int index = 0>
struct FIND
{
   enum { VAL = LIST::HEAD == value? 
                index: 
                FIND <LIST::TAIL, value, index+1> ::VAL };
};

template <int value, int index>
struct FIND <void, value, index>
{
   enum { VAL = NAN };
};



template <typename LIST1, typename LIST2>
struct SET_MINUS
{
   typedef  IF < FIND <LIST2, LIST1::HEAD> ::VAL != NAN,
                 SET_MINUS <LIST1::TAIL, LIST2>::RET,
                 ILIST <LIST1::HEAD, SET_MINUS <LIST1::TAIL, LIST2> ::RET>::RET 
               >::RET  RET;
};

template <typename LIST2>
struct SET_MINUS <void, LIST2>
{
   typedef  void  RET;
};



template <int beg, int end, int step = 1>
struct RANGE
{
   typedef  IF <(end - beg)/step > 0,
               ILIST <beg, RANGE <beg + step, end, step> ::RET>::RET,
               void
            >::RET  RET;
};



//template <class T0, class T1 = void, class T2 = void, class T3 = void, class T4 = void>
//struct is_same: public 
//   and <std::tr1::is_same <T0, T1>,  
//        std::tr1::is_same <T0, T2>,
//        std::tr1::is_same <T0, T3>,
//        std::tr1::is_same <T0, T4>>  {};
//

template <class Base, template <class T> class Extend, bool is_active = false>
struct  Extension  {};




template <class Base, template <class T> class Extend>
struct  Extension <Base, Extend, true>: public Extend <Base> {};

template <class T>  class no_extension {};


template 
<
   template <class> class T1 = no_extension, 
   template <class> class T2 = no_extension, 
   template <class> class T3 = no_extension, 
   template <class> class T4 = no_extension
>
struct Extensions
{
   template <class Base>
   class type: public T1<Base>, public T2<Base>, public T3<Base>, public T4<Base>  {};
};


template 
<
   template <class> class T1, 
   template <class> class T2, 
   template <class> class T3
>
struct Extensions <T1, T2, T3, no_extension>
{
   template <class Base>
   class type: public T1<Base>, public T2<Base>, public T3<Base>  {};
};


template 
<
   template <class> class T1, 
   template <class> class T2
>
struct Extensions <T1, T2, no_extension, no_extension>
{
   template <class Base>
   class type: public T1<Base>, public T2<Base>  {};
};

template 
<
   template <class> class T1
>
struct Extensions <T1, no_extension, no_extension, no_extension>
{
   template <class Base>
   class type: public T1<Base>  {};
};

template <>
struct Extensions <no_extension, no_extension, no_extension, no_extension>  
{
   template <class Base>
   class type {};
};





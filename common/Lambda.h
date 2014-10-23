#ifndef  _LAMBDA__
#define  _LAMBDA__

#include <vector>
#include <Expressions.h>




namespace lambda {


namespace _ {

class lambda_tag {};

template <int pos>
struct  Argument;

template <>
struct  Argument<1>
{
   static const int rank = 0;

   template <class T1>
   T1&  operator() (T1& val, ...) const
   {
      return val;
   }

   template <class T1>
   const T1&  operator() (const T1& val, ...) const
   {
      return val;
   }
};


template <>
struct  Argument<2>
{
   static const int rank = 0;

   template <class T1, class T2>
   T2&  operator() (T1, T2& val, ...) const
   {
      return val;
   }

   template <class T1, class T2>
   const T2&  operator() (T1, const T2& val, ...) const
   {
      return val;
   }
};


template <>
struct  Argument<3>
{
   static const int rank = 0;

   template <class T1, class T2, class T3>
   T3&  operator() (T1, T2, T3& val, ...) const
   {
      return val;
   }

   template <class T1, class T2, class T3>
   const T3&  operator() (T1, T2, const T3& val, ...) const
   {
      return val;
   }
};





template <class T>
struct  Var
{
   static const int rank = 0;

   T value;
   Var (const T& val): value(val)  {};

   const T&  operator() (...) const
   {
      return value; 
   }
};


template <class T>
struct  Ref
{
   static const int rank = 0;

   T& value;
   Ref (T& val): value(val)  {};

   T&  operator() (...) const
   {
      return value; 
   }
};


template <class U, class V>
struct Assign: private BinaryExpression <U, V>
{
   Assign (const U& lhs, const V& rhs): BinaryExpression (lhs, rhs)  {}


   auto operator() () const -> decltype (left())
   {
      return  left() = right();
   }

   template <class T1>
   auto operator() (T1& t1) const -> decltype (left(t1))
   {
      return  left (t1) = right (t1);
   }

   template <class T1, class T2>
   auto operator() (T1& t1, T2& t2) const -> decltype (left(t1, t2))
   {
      return  left (t1, t2) = right (t1, t2);
   }

   template <class T1, class T2, class T3>
   auto operator() (T1& t1, T2& t2, T3& t3) const -> decltype (left(t1, t2, t3))
   {
      return  left (t1, t2, t3) = right (t1, t2, t3);
   }
};

} // namespace _


template <class T>
struct Lambda: public T, public _::lambda_tag
{
   typedef  Lambda  Super;

   Lambda()  {}

   Lambda (const T& val): T(val)  {}

   template <class U>
   Lambda (U& child): T(child)  {}

   template <class U, class V>
   Lambda (U& lhs, V& rhs): T(lhs, rhs)  {}


   template <class V>
   Lambda<_::Assign<T, V>> operator= (const Lambda<V>& other)
   {
      return   Lambda<_::Assign<T, V>> (*this, other);
   }
};





template <class T>
Lambda<_::Var<T>>  var (const T& val)
{
   return  Lambda<_::Var<T>> (val);
}


template <class T>
Lambda<_::Ref<T>>  ref (T& val)
{
   return  Lambda<_::Ref<T>> (val);
}

template <class T>
Lambda<_::Ref<const T>>  ref (const T& val)
{
   return  Lambda<_::Ref<const T>> (val);
}


const Lambda <_::Argument<1>> x1;
const Lambda <_::Argument<2>> x2;



} // namespace mgo
#endif

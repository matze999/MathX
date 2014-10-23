#ifndef __FN_
#define __FN_


namespace mgo { namespace fn {

struct  plus
{
   template <class argT1, class argT2>
   auto operator() (const argT1 &lhs, const argT2 &rhs) const  -> decltype (lhs + rhs)  
   {
      return lhs + rhs;
   }
};

struct  minus
{
   template <class argT1, class argT2>
   auto operator() (const argT1 &lhs, const argT2 &rhs) const  -> decltype (lhs - rhs)  
   {
      return lhs - rhs;
   }
};

struct  multiply
{
   template <class argT1, class argT2>
   auto operator() (const argT1 &lhs, const argT2 &rhs) const  -> decltype (lhs * rhs)  
   {
      return lhs * rhs;
   }
};

struct  divide
{
   template <class argT1, class argT2>
   auto operator() (const argT1 &lhs, const argT2 &rhs) const  -> decltype (lhs / rhs)  
   {
      return lhs / rhs;
   }
};

struct  uminus
{
   template <class argT>
   argT  operator() (const argT &arg) const
   {
      return -arg;
   }
};


//*** predicates

struct equal 
{
   template <class T1, class T2> 
   bool  operator() (const T1 &lhs, const T2 &rhs) const    { return lhs == rhs; }
};


struct neq 
{
   template <class T1, class T2> 
   bool  operator() (const T1 &lhs, const T2 &rhs) const    { return lhs != rhs; }
};


struct gt 
{
   template <class T1, class T2> 
   bool  operator() (const T1 &lhs, const T2 &rhs) const    { return lhs > rhs; }
};


struct geq 
{
   template <class T1, class T2> 
   bool  operator() (const T1 &lhs, const T2 &rhs) const    { return lhs >= rhs; }
};


struct lt
{
   template <class T1, class T2> 
   bool  operator() (const T1 &lhs, const T2 &rhs) const    { return lhs < rhs; }
};


struct leq 
{
   template <class T1, class T2> 
   bool  operator() (const T1 &lhs, const T2 &rhs) const    { return lhs <= rhs; }
};


struct  logical_and
{
   bool  operator() (bool lhs, bool rhs) const    { return lhs && rhs; }
};


struct  logical_or
{
   bool  operator() (bool lhs, bool rhs) const    { return lhs || rhs; }
};


}} // namespace fn and mgo
#endif



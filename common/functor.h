#ifndef __FUNCTOR_
#define __FUNCTOR_

#include <meta.h>


namespace mgo {

   
template <class T>  class Functor;


namespace {  // unnamed
using  namespace ::mp;

//*** Pointer to function

template <class Res>
struct Functor <Res(*)()>: public function_traits <Res()>
{
   typedef  Res(*functor_type)();

   Functor (functor_type fun): funptr(fun)  {}

   Res  operator() () const
   {
      return  (*funptr) ();
   }

protected:
   functor_type  funptr;
};



template <class Res, class A1>
struct Functor <Res(*)(A1)>: public function_traits <Res(A1)>
{
   typedef  Res(*functor_type)(A1);  

   Functor (functor_type fun): funptr(fun)  {}

   Res  operator() (A1  arg1) const 
   {
      return  (*funptr) (arg1);
   }

protected:
   functor_type  funptr;
};


template <class Res, class A1, class A2>
struct Functor <Res(*)(A1, A2)>: public function_traits <Res(A1, A2)>
{
   typedef  Res(*functor_type)(A1, A2);   

   Functor (functor_type fun): funptr(fun)  {}

   Res  operator() (A1  arg1, A2  arg2) const 
   {
      return  (*funptr) (arg1, arg2);
   }

protected:
   functor_type  funptr;
};



//*** Pointer to member function

template <class Res, class Base>
struct Functor <Res (Base::*)()>: public function_traits <Res(Base&)>
{
   typedef  Res(Base::*functor_type)(); 

   Functor (functor_type fun): memfun(fun)  {}

   Res  operator() (Base  &arg1) const
   {
      return  (arg1.*memfun) ();
   }

private:
   functor_type  memfun;
};


template <class Res, class Base>
struct Functor <Res (Base::*)() const>: public function_traits <Res(const Base&)>
{
   typedef  Res(Base::*functor_type)() const; 

   Functor (functor_type fun): memfun(fun)  {}

   Res  operator() (const Base  &arg1) const
   {
      return  (arg1.*memfun) ();
   }

private:
   functor_type  memfun;
};


template <class Res, class Base, class T1>
struct Functor <Res (Base::*)(T1)>: public function_traits <Res(Base&, T1)>
{
   typedef  Res(Base::*functor_type)(T1); 

   Functor (functor_type fun): memfun(fun)  {}

   Res  operator() (Base &arg1, T1 arg2) const
   {
      return  (arg1.*memfun) (arg2);
   }

private:
   functor_type  memfun;
};


template <class Res, class Base, class T1>
struct Functor <Res (Base::*)(T1) const>: public function_traits <Res(const Base&, T1)>
{
   typedef  Res(Base::*functor_type)(T1) const; 

   Functor (functor_type fun): memfun(fun)  {}

   Res  operator() (const Base &arg1, T1 arg2) const
   {
      return  (arg1.*memfun) (arg2);
   }

private:
   functor_type  memfun;
};



//*** Pointer to member object

template <class Res, class Base>
struct Functor <Res Base::*>: public function_traits <Res& (Base&)>
{
   typedef  Res  Base::*functor_type; 

   Functor (functor_type obj): memobj(obj)  {}

   Res&  operator() (Base&  arg1) const 
   {
      return  arg1.*memobj;
   }

   const Res&  operator() (const Base& arg1) const 
   {
      return  arg1.*memobj;
   }

private:
   functor_type  memobj;
};


template <class function, bool = or <is_member_pointer <function>, 
                                     is_function_pointer <function>>::value>
struct make_functor
{
   typedef  function  type;
};
 
template <class function>
struct make_functor <function, true>
{
   typedef  Functor <function>  type;
};

} // unnamed
} // namespace mgo
#endif 


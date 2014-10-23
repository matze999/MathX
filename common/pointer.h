#ifndef  _POINTER_H_
#define  _POINTER_H_

#include <util.h>


namespace mgo {


namespace  Virtue { enum type 
{
   Autonomous,
   Shared
};}


template <class Base, Virtue::type = Virtue::Shared>
class Pointer
{
public:
   Pointer (): data(nullptr)  {}
   Pointer (Base *other): data(other)  {}

   operator bool()                                 { return data != nullptr; }

         Base*  operator-> ()                      {  return  data; }
   const Base*  operator-> () const                {  return  data; }

         Base&  operator* ()                       {  return  *data; }
   const Base&  operator* () const                 {  return  *data; }

   Pointer&  operator= (Base *other)
   {
      data = other;
      return  *this;
   }

protected:
   Base  *data;
};



// constraint!
// initialize this class with a pointer, that should be relesed inside the destructor
template <class Base>
class Pointer <Base, Virtue::Autonomous>
{
public:
   Pointer ()  
   {
      data = new Base;
   }
   Pointer (Base *other)  
   {
      data = other;
   }

   ~Pointer()
   {
      delete data;
   }

   Pointer&  operator= (Base *other)
   {
      delete data;
      data = other;
      return  *this;
   }


   Pointer&  operator&= (Base *other)
   {
      delete data;
      data = new Base (*other);
      return  *this;
   }

   operator bool()                                 { return true }

         Base*  operator-> ()                      {  return  data; }
   const Base*  operator-> () const                {  return  data; }

         Base&  operator* ()                       {  return  *data; }
   const Base&  operator* () const                 {  return  *data; }

protected:
   Base  *data;
};


template <class Base>
class  SharedPointer
{
public:
   SharedPointer (): construct_by_myself (true)
   {
      data = new Base;
   }

   SharedPointer (Base &base): data (&base), construct_by_myself (false)  {}

   ~SharedPointer()
   {
      clear();
   }


   Base*  operator-> ()
   {
      return data;
   }

   const Base*  operator-> () const
   {
      return data;
   }


   SharedPointer&  operator= (Base &base)
   {
      clear();
      data = &base;
      construct_by_myself = false;
   }


   SharedPointer&  operator&= (Base &base)
   {
      clear();
      data = new Base(base);
      construct_by_myself = true;
   }


private:
   void clear()
   {
      if (construct_by_myself)  delete data;
   }

   SharedPointer (const SharedPointer&);
   void operator= (const SharedPointer&);

   Base  *data;
   bool  construct_by_myself;
};



} // namespace mgo
#endif

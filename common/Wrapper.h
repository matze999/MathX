#ifndef __WRAPPER_H_
#define __WRAPPER_H_


#include "Allocator.h"
#include "Reference.h"
#include "meta/typetraits.h"


namespace mgo {

enum class Assoc: char {
   Reference,                 // reference to an other object
   WeakReference,             // reference to an other object without changing the other object
   Composite,                 // points to own data that is released at destructor time
   AutoReference              // points to own data that is released at program end
};




template <class Base, Assoc assoc = Assoc::Reference>
class Wrapper: public Reference <Base>
{
public:
   Wrapper()  {}
   Wrapper (Base& other): Reference<Base> (other)  {}

   using Reference::operator=;
};



template <class Base>  
class Wrapper <Base, Assoc::Composite>: public Reference <Base>
{
public:
   template <class ...Args>
   Wrapper (Args&&... args)
   {
      data = new Base (args...);
   }

   Wrapper (const Reference<Base>& other)
   {
      data = new Base (other.get ());
   }

   Wrapper (const Wrapper& other)
   {
      data = new Base (other.get());
   }

   // create the object with new, i.e.  wrapper = new Base;
   Wrapper (Base* obj)              { data = obj; }  

   ~Wrapper()                       { delete data;}


   using  Reference<Base>::operator=;


   // only for compatibility
   void  reassign (const Base& other)                 { *data = other; }
   void  reassign (const Reference <Base>& other)     { *data = other.get(); }
};



template <class Base>  
class Wrapper <Base, Assoc::WeakReference>: public Reference <Base>
{
public:
   Wrapper()  {}
   Wrapper (Base& other): Reference<Base> (other)  {}

   Wrapper (Base* obj): is_allocated (true)   // create own instance with new
   {
      data = obj;
   }

   Wrapper (const Wrapper& other):
      Reference <Base> (other.get()), is_allocated (false)  {}

   ~Wrapper()                                      
   {
      if (is_allocated)  delete data;
   }


   void  reassign (Base& other)
   {
      clear ();
      data = &other;
   }

   void  reassign (Reference<Base>& other)
   {
      reassign (other.get());
   }

   void  reassign (Base* other)   // create own instance with new
   {
      clear();
      data = other;  
      is_allocated = true;
   }


   using  Reference<Base>::operator=;

   void  detach()
   {
      if (!is_allocated)
      {
         assert (data);
         data = new Base (*data);
         is_allocated = true;
      }
   }

   void clone ()     { detach (); }

private:
   void  initData() 
   {
      if (!data)
      {
         data = new Base;
         is_allocated = true;
      }
   }

   void  clear()
   {
      if (is_allocated) 
      {
         delete data;
         is_allocated = false;
      }
   }

   bool  is_allocated;
};


template <class Base>
using  WeakReference = Wrapper <Base, Assoc::WeakReference>;



template <class Base>  
class Wrapper <Base, Assoc::AutoReference>: public Reference <Base>
{
public:
   Wrapper ()                       { reassign(); }
   Wrapper (Wrapper& other)         { data = other.data; }    // move constructor
   Wrapper (Base* obj)              { allocator.registerObject (data = obj); }


   void  reassign ()
   {
       allocator.registerObject (data = new Base); 
   }

   void  reassign (Base& other)
   {
      allocator.registerObject (data = new Base(other)); 
   }

   void  reassign (Wrapper& other)
   {
      allocator.registerObject (data = new Base(other.data)); 
   }

   using Reference <Base>::operator=;

private:
   static Allocator<Base>   allocator;
};


template <class Base>  
Allocator<Base>  Wrapper<Base, Assoc::AutoReference>::allocator;





//*** operators for all Wrapper templates

template <class Base, Assoc assoc, class T>
Wrapper <Base, assoc>&  operator <<  (Wrapper <Base, assoc>&  object, const T& val)
{
   object.get() << val;
   return  object;
}


template <class Base, Assoc assoc, class T>
Wrapper <Base, assoc>&  operator +=  (Wrapper <Base, assoc>&  object, const T& val)
{
   object.get () += val;
   return  object;
}



template <class T, Assoc assoc>
bool  operator==  (const Wrapper <T, assoc>&  ref, const T& val)
{
   return  ref.get () == val;
}

template <class T, Assoc assoc>
bool  operator !=  (const Wrapper <T, assoc>&  ref, const T& val)
{
   return  ref.get () != val;
}



} // namespace mgo
#endif 

#ifndef __REFERENCE_H_
#define __REFERENCE_H_

#include "meta/subscript_result.h"


namespace mgo {



template <class Base>
class Reference
{
public:
   Reference (): data (nullptr)  {}
   Reference (Base& other): data (&other)  {}


   const Reference&  operator= (const Base& other) const             
   { 
      get() = other;  
      return  *this; 
   }

   const Reference&  operator= (const Reference& other) const 
   {
      get() = other.get();  
      return  *this; 
   }


   void  reassign (Base& other)                 { data = &other; }
   void  reassign (Reference& other)            { data = other.data; }

   bool  is_valid() const                       { return data != nullptr; }
   bool  isInitialized() const                  { return data != nullptr; }
   operator bool() const                        { return data != nullptr; }

   Base&  Value () const                        { assert (data); return *data; }

   Base&  get () const                          { assert (data); return *data; }
   Base*  operator-> () const                   { assert (data); return data; }

   operator  Base& () const                     { return  get(); }


// ***  Container methods
   template <class T>
   void  push_back (T&& val) const              { get().push_back (val); }

   template <class ...Args>
   void  emplace_back (Args&&... args) const    { get().emplace_back (args...); }

   bool  empty() const                          { return  get().empty(); }
   size_t  size() const                         { return  get().size(); }
   void  clear() const                          { get().clear(); }


   //template <class Arg>
   //typename mp::subscript_result<Base, Arg>::type  operator[] (Arg pos) const
   //{
   //   return  get()[pos];
   //}

protected:
   Base  *data;
};


//template <class Base>
//class Reference <const Base>
//{
//public:
//   Reference (): data (nullptr)  {}
//   Reference (const Base& other): data (&other)  {}
//
//   Reference&  operator= (Reference& other) = delete;
//
//   void  reassign (const Base& other)                 { data = &other; }
//   void  reassign (const Reference& other)            { data = other.data; }
//
//
//   bool  is_valid () const                            { return data != nullptr; }
//
//   const Base&  get () const                          { assert (data); return *data; }
//
//   const Base*  operator-> () const                   { return  &get(); }
//
//   operator const Base& () const                      { return  get (); }
//
//
//   // ***  Container methods
//   size_t  size () const                              { return  get().size(); }
//   bool    empty () const                             { return  get().empty(); }
//
//   //template <class Arg>
//   //typename mp::subscript_result<const Base, Arg>::type  operator[] (Arg pos) const
//   //{
//   //   return  get() [pos];
//   //}
//
//protected:
//   const Base*  data;
//};
//

//*** operators 

template <class ostream, class T>
ostream&  operator<< (ostream& out, const Reference<T>& ref)
{
   out << ref.get();
   return out;
}


template <class T>
bool  operator ==  (const Reference <T>&  ref, const T& val)
{
   return ref.get() == val;
}

template <class T>
bool  operator !=  (const Reference <T>&  ref, const T& val)
{
   return ref.get() != val;
}



//*** specialisation for pointers

template <class Base>
class Reference <Base*>
{
public:
   Reference (): data (new Base*)  { *data = nullptr; }

   Reference (Base* other): data (&other)  {}

   Reference (const Reference& other): data (other.data)  {}


   void  reassign (Base* other)             
   { 
      data = &other;
   }

   void  reassign (Reference& other) 
   {
      data = other.data;
   }

   Reference&  operator= (Base* other)               
   { 
      Value() = other;  
      return  *this; 
   }

   Reference&  operator= (const Reference& other)  
   {
      if (data != other.data)    Value() = other.Value();  
      return  *this; 
   }


         Base*&  Value()                        { assert (data); return *data; }
   const Base*   Value() const                  { assert (data && *data); return *data; }

         Base*  operator-> ()                   { return  Value(); }
   const Base*  operator-> () const             { return  Value(); }


         Base&  operator* ()                      { return  *Value(); }
   const Base&  operator* () const                { return  *Value(); }

   operator       Base* ()                      { return  Value(); }
   operator const Base* () const                { return  Value(); }

protected:
   Base  **data;
};

} // namespace mgo
#endif 

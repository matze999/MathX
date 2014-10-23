#ifndef __MGO_OPTIONAL_H
#define __MGO_OPTIONAL_H



namespace mgo {

template <class T>
class Optional
{
public:

   Optional ():  is_initialized (false)  {}
   Optional (const T& val): value (val), is_initialized (true)  {}

   Optional (T&& val): value (val), is_initialized (true)  {}

   Optional (bool): is_initialized (false)  {}


   operator bool ()
   {
      return is_initialized;
   }

   bool isInitialized ()
   {
      return is_initialized;
   }

   operator T&()
   {
      return value;
   }

   T&  get ()
   {
      return value;
   }
   const T&  get () const
   {
      return value;
   }
private:
   T     *value;
   bool  is_initialized;
};


} // namespace mgo
#endif

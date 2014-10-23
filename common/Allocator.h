#ifndef __POINTER_POOL_H
#define __POINTER_POOL_H

#include "util.h"



namespace mgo {

template <class Base>
class Allocator
{
public:

   ~Allocator()
   {
      with (data_pool)  delete data_pool [pos_];
   }


   Base*  newObject() 
   {
      Base* obj = new Base;
      data_pool.push_back (obj);
      return obj;
   }

   template <class T, class ...Args>
   Base*  newObject (T&& val, Args&&... args) 
   {
      Base* obj = new Base (val, args...);
      data_pool.push_back (obj);
      return adr;
   }

   Base*  registerObject (Base* obj) 
   {
      data_pool.push_back (obj);
      return obj;
   }

   void*  createObject (size_t size)
   {
      void *adr = malloc (size);
      data_pool.push_back ((Base*) adr);
      return obj;
   }


   void  deleteObject (void *adr)
   {
      iterator it = std::find (data_pool.begin(), data_pool.end(), (Base*) adr);
      if (it != end())
      {
         free (adr);
         *it = nullptr;
      }
   }

   size_t  getObjectNumber() const
   {
      return  data_pool.size();
   }

private:
   std::vector <Base*>  data_pool;
};




} // namespace mgo
#endif

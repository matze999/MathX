#ifndef __MUTEX_
#define __MUTEX_

#include <windows.h>

namespace mgo {

class mutexT
{
public:
   mutexT()
   {
      InitializeCriticalSection (&section);
   }

   ~mutexT()
   {
      DeleteCriticalSection (&section);
   }

   void  lock()  
   {
      EnterCriticalSection (&section); 
   }

   void  unlock() 
   {
      LeaveCriticalSection (&section); 
   }


   class sectionT
   {
      mutexT  &mutex;
   public:
      sectionT (mutexT& mtx): mutex (mtx)
      {
         mutex.lock();
      }

      ~sectionT()
      {
         mutex.unlock();
      }

      operator bool()
      {
         return true;
      }
   };

private:
   CRITICAL_SECTION  section;
};


#define critical_section(mutex)  \
   if (mutexT::sectionT  section = mutex)


} // namespace mgo
#endif // __MUTEX_
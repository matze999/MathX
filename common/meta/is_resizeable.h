#ifndef __ISRESIZEABLE_
#define __ISRESIZEABLE_


#include <vector>


namespace mgo { namespace mp {


template <class T>
struct is_resizeable
{
   template <class Base>
   static
   char resize_test (std::vector <Base> *);

   static
   int resize_test (...);

   static
   T* instance();

   static const bool  value = (sizeof resize_test (instance()) == 1);
};


}} // namespace mgo + mp
#endif


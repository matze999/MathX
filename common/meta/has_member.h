#ifndef __HAS_MEMBER_H_
#define __HAS_MEMBER_H_

#include "typetraits.h"

namespace  mp {



template <class T>
class has_member
{
   static T&  Instance ();

   template <class U>
   static typename U::type  get_type (U&);
   static any_type   get_type (...);

public:
   typedef decltype (get_type (Instance ()))  type;
   static const bool value = !is_same <type, any_type>::value;
};


} // namespace mp
#endif

#ifndef __META__
#define __META__

#include "meta/if.h"
#include "meta/typetraits.h"
#include "meta/typeinfo.h"



template <class Base, class Value>
struct member_value
{
   typedef Value Base::*  type;
};



#endif // __META_



#ifndef __SUBSCRIPT_RESULT_H_
#define __SUBSCRIPT_RESULT_H_

#include "typetraits.h"

namespace  mp {


template <class T, class S>
struct subscript_result
{
   using type = decltype (Instance<T>() [Instance<S>()]);
};


} // namespace mp
#endif

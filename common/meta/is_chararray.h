#ifndef __IS_CHARARRAY_H_
#define __IS_CHARARRAY_H_

#include "typetraits.h"

namespace  mp {

   
template <class T>
struct is_chararray: public 
   is_same <typename std::remove_extent <
            typename std::remove_reference <T>::type>::type, 
            const char>  {};


template <class T, bool = is_chararray<T>::value>
struct make_proper
{
   typedef const char* type;
};

template <class T>
struct make_proper <T, false>
{
   typedef T type;
};


} // namespace mp
#endif

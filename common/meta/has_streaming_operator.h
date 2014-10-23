#ifndef __HAS_STREAMING_OPERATOR_H_
#define __HAS_STREAMING_OPERATOR_H_

#include "typetraits.h"
#include "iosfwd"
//#include <ostream>


namespace  mp {

namespace _ {  // protect visibility from namespace mp

template <class T, class dummy = void>
class has_streaming_operator_imp
{
   friend
   any_type  operator<< (std::ostream&, const any_type&);
   
public:
   static const bool value = 
      !is_same <decltype (Instance <std::ostream>() << Instance <T>()),
                any_type>::value;
};



// exclude 'ostream' from check
template <class T>
class has_streaming_operator_imp
   <T, typename enable_if <is_base_of <ostream, T>::value>::type>
{
public:
   static const bool value = false;
};

//
//template <class T>
//class has_streaming_operator <T&>: public has_streaming_operator <T> {};
//
//template <class T>
//class has_streaming_operator <const T&>: public has_streaming_operator <T> {};
//
} // namespace _


template <class T>
class has_streaming_operator: public _::has_streaming_operator_imp<T>  {};


} // namespace mp
#endif

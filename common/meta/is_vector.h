#ifndef __IS_VECTOR_H_
#define __IS_VECTOR_H_


#include <vector>

namespace  mp {

template <class T>
class  is_vector
{
   template <class U>
   static  char  check (std::vector<U> &);

   static  int  check (...);

   static  T&  Instance();

public:
   static const bool value = sizeof (check (Instance())) == 1; 
};

} // namespace mp
#endif

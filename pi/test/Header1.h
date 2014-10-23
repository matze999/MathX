#ifndef __CLEAR_FUN
#define __CLEAR_FUN

#include "meta/if.h"


namespace fn {


template <class T>
struct vector_cleaner
{
   static  void execute (T& value)
   {
      value.clear();
   }
};

template <class T>
struct num_cleaner
{
   static  void execute (T& value)
   {
      value = 0;
   }
};


template <class T>
struct cleaner: public 
   if_ <is_arithmetic<T>::value, num_cleaner <T>,
                                 vector_cleaner <T>>::type  
{};



template <class T>
void  clear (T& val)
{
   cleaner <T>::execute (val);
}

//template <class T>
//typename enable_if 
//<
//   or <::mp::is_vector<T>, is_base_of<std::string, T>>::value, 
//   void
//>::type
//clear (T&  value)
//{
//   value.clear();
//}
//
//
//template <class T>
//typename enable_if <is_arithmetic<T>::value, void>::type
//clear (T&  value)
//{
//   value = 0;
//}
//
//template <class T, Assoc assoc>
//void  clear (Attribute <T, assoc>&  attrib)
//{
//   clear (attrib.Value());
//}


} // namespace fn
#endif

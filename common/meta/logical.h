#ifndef __META_LOGICAL__
#define __META_LOGICAL__



namespace mp {


template <class T>
struct not_
{
   static const bool value = !T::value;
};



template <class T1, class T2, bool first_value = T1::value>
struct and_
{
   static const bool value = false;
};

template <class T1, class T2>
struct and_<T1, T2, true>
{
   static const bool value = T2::value;
};



template <class T1, class T2, bool first_value = T1::value>
struct or_
{
   static const bool value = true;
};

template <class T1, class T2>
struct or_<T1, T2, false>
{
   static const bool value = T2::value;
};


} // namespace mp
#endif


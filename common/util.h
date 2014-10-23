
#ifndef __UTIL_
#define __UTIL_

#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <CRTDBG.h>
#include <tchar.h>


#pragma warning (disable: 4996 4305 4244 4800 4267)



/// Arguments number in a variadic macro
#define VA_NARG(...)      VA_EXPAND(VA_NARG_(__VA_ARGS__, 5, 4, 3, 2, 1, 0))


#define VA_NARG_(_1, _2 ,_3 ,_4 ,_5, N, ...)  N
#define VA_EXPAND(...)   __VA_ARGS__ 


#define _in
#define _out


/**
   loop over all elements of a container
   works best with vector like types
   use pos_ and last_ to control the loop

   with (int &value, int_vector)
   {
      value = 42;         // access the actual element
      ...
      if (pos_ == last_)  // last element in int_vector
         ...
   }
*/


#define with(...)  VA_EXPAND(with__(VA_NARG(__VA_ARGS__))(__VA_ARGS__))

#define with__(num)  with_0 (num)
#define with_0(num)  with_##num


#define with_1(buf)  \
   for (size_t pos_ = 0; pos_ < (buf).size(); ++pos_)

#define with_2(el, buf)  \
   for (size_t pos_ = 0, last_ = (buf).size()-1; pos_ < (buf).size(); ++pos_)\
      if (bool doit = true)\
         for (el = (buf)[pos_]; doit; doit = false) 


/*
   with_front (int_vector)  cout << front_;

   with_tail (int_vector)  cout << ", " << tail_;

*/

#define with_front(buf)  \
   if (bool doit = !(buf).empty()) \
      for (auto front_ = (buf)[0]; doit; doit = false) 

#define with_tail(buf)  \
   for (size_t pos_ = 1, end_ = (buf).size(); pos_ < end_; ++pos_)\
      if (bool doit = true)\
         for (auto tail_ = (buf)[pos_]; doit; doit = false) 



//*** static errors
class STATIC_FAILURE  {};

template <int ERR_CODE_, typename ERROR_= STATIC_FAILURE>
struct STATIC_CHECK;

template <typename ERROR_>
struct STATIC_CHECK <0, ERROR_>  {};


#define define_static_error(err)      class  err {}
#define static_check(cond, err)       sizeof  STATIC_CHECK <cond, err>
#define static_check0(cond)           sizeof  STATIC_CHECK <cond, STATIC_FAILURE>


#if _MSC_VER >= 1600
   #define static_assert0(cond)       static_assert (cond, "A invalid state occured")

#else // static_assert ist not defined
   #define static_assert(cond, err)   sizeof  STATIC_CHECK <!(cond), err>
   #define static_assert0(cond)       sizeof  STATIC_CHECK <!(cond), STATIC_FAILURE>
#endif

#if _MSC_VER >= 1800
#define _STUDIO_2013
#endif


template <class TYPE_> class STATIC_PRINT_ ;// { static const char _ = 1234; };
template <int VALUE_>  class STATIC_VALUE_  { static const char _= 1234; };


#define  static_print(T)          enum { type  = sizeof typename STATIC_PRINT_<T>::type }
#define  static_value(val)        enum { value = sizeof STATIC_VALUE_<val> }



//*** DEBUG make
#ifndef NDEBUG

__inline
bool assert_imp (bool cond, const char *filename, int linenumber, const char *err)
{
   if (!cond) //  && _CrtDbgReport (_CRT_ASSERT, filename, linenumber, NULL, err))
      _CrtDbgBreak(); 
   return cond;
}


#undef   assert
#define  assert(cond)        if (!(cond)) _CrtDbgBreak(); 
#define  assert0(cond)       if (!(cond)) _CrtDbgBreak(); 

#define  debug_code          if (true)



//*** RELEASE make 
#else 

#define  assert(cond)   
#define  assert0(cond)  

#define  debug_code          if (false)

#endif
//*** END make type


#define  self  (*this)


typedef  unsigned int  uint;
typedef  unsigned char byte;


#undef min
#undef max

template <class T>
const T&  min (const T& lhs, const T& rhs)
{
    return  lhs < rhs? lhs: rhs;
}

template <class T>
const T&  max (const T& lhs, const T& rhs)
{
    return  lhs < rhs? rhs: lhs;
}


namespace _{


template <class T, class Distance = void>
struct Range0: public Range0<T>
{
   Range0 (const T& a, const T& b, Distance step): Range0<T> (a, b), dist(step)  {}

   bool contains (const T& value) const
   {
      return  Range0<T>::contains (value)  &&  ((value - first) % dist == 0);
   }

   Distance dist;
};


template <class T>
struct Range0 <T, void>
{
   Range0 (const T& a, const T& b): first(a), last(b)  {}

   bool contains (const T& value) const
   {
      return  first <= value  &&  value <= last;
   }

   T first, last;
};



template <class T>
Range0<T>  Range (const T& a, const T& b)
{
   return  Range0<T> (a, b);
}

inline
Range0<int> Range (int a, int b)
{
   return Range<int> (a, b);
}


template <class T, class Distance>
Range0 <T, Distance>  Range (const T& a, const T& b, Distance dist)
{
   return  Range0 <T, Distance> (a, b, dist);
}

inline
Range0<int, int> Range (int a, int b, int s)
{
   return Range<int, int> (a, b, s);
}


template <class Value, class T, class Distance>
bool operator% (const Value& value, const Range0 <T, Distance>& range)
{
   return range.contains (value);
}


} // namespace _


inline
bool in (int val, int max)
{
   return  val >= 0  && val <= max;
}

inline
bool  in (int val, int min, int max)
{
   return  val >= min  &&  val <= max;
}

#define inside(...)  VA_EXPAND(in__(VA_NARG(__VA_ARGS__))(__VA_ARGS__))

#define in__(num)  in_0 (num)
#define in_0(num)  in_##num

#define in_1(b)   % _::Range(0, b)
#define in_2(a,b)   % _::Range(a, b)
#define in_3(a,b,s)   % _::Range(a, b, s)



struct  AnyValue
{
   operator int() const
   {
      return  0x7fffffff;

   }

   //operator size_t() const
   //{
   //   return  0xffffffff;
   //}
};

const AnyValue any;




#define  using_namespace(name)  namespace { using namespace name;
#define  end_using(name)  }



namespace std {

template <class T>
typename  std::tr1::enable_if <std::tr1::is_integral<T>::value, std::string&>::type
operator+= (std::string &data, T value)
{
   std::stringstream  stream;
   stream << value;
   return data += stream.str();
}


template <class T>
typename  std::tr1::enable_if <std::tr1::is_integral<T>::value, std::string>::type
operator+ (const std::string &data, T value)
{
   std::stringstream  stream;
   stream << value;
   return data + stream.str();
}

} // namespace std



namespace mgo {


#ifdef  _UNICODE
   typedef  std::wstring String;
   #define cU(text)  L##text
#else 
   typedef  std::string  String;
   #define cU(text)   text
#endif






class  exceptionT
{
public:
   exceptionT (const TCHAR *mess, int code = 0)
   {
      error_message = mess;
      error_code    = code;
   }

   exceptionT (const TCHAR *file, int line, const TCHAR *mess, int code = 0)
   {
      setLocation (file, line);
      error_message = mess;
      error_code    = code;
   }

   exceptionT (const String &mess, int code = 0)
   {
      error_message = mess;
      error_code    = code;
   }
   
   void setLocation (const TCHAR *file, int line)
   {
      _stprintf (where, cU("%s (%d)"), file, line);
   }
   
   const String&  Text() const
   {
      return  error_message;
   }
   
   String LongText() const
   {
      return  error_message + cU("\nLocation: ") + where;
   }
   
   int  ErrorCode()
   {
      return error_code;
   }
   
private:
   String  error_message;
   int      error_code;
   TCHAR  where[100];
};


#define  throw_if(cond, exception)  \
   if (cond) {\
      exception.setLocation (_T(__FILE__), __LINE__); \
      throw exception; }

#define Exception(text)  exceptionT (_T(__FILE__), __LINE__, cU(#text))



class Timer
{
   clock_t     start_time, mean_time;
public:
   Timer()    
   {
      start_time = mean_time = clock(); 
   }

   size_t MeanTime()
   {
      double  last_time = mean_time;
      mean_time = clock();
      return  size_t(mean_time - last_time) *1000/ CLOCKS_PER_SEC;
   }

   size_t TotalTime()
   {
      mean_time = clock();
      return  size_t(mean_time - start_time) *1000/ CLOCKS_PER_SEC;
   }

   void start()
   {
      start_time = mean_time = clock(); 
   }
};




class  CyclicIndex
{
   uint  value, max_value;
public:
   CyclicIndex(): max_value(0xffffffff) {}

   CyclicIndex (size_t val, size_t max = 0xffffffff): value(val), max_value(max)   {}

   void  setMaxValue (size_t max)
   {
      max_value = max;
   }

   CyclicIndex&  operator= (size_t val)
   {
      value = val  % max_value;
      return *this;
   }


   uint operator++ ()
   {
      return  value = (value + 1) % max_value;
   }

   uint operator-- ()
   {
      if (value)  --value;
      else value = max_value - 1;
      return  value;
   }

   uint operator+ (uint n)
   {
      return  (value + n) % max_value;
   }

   uint operator- (uint n)
   {
      return  (value + max_value - n) % max_value;
   }

   void operator+= (uint n)
   {
      value = (value + n) % max_value;
   }

   void operator-= (uint n)
   {
      value = (value + max_value - n) % max_value;
   }

   operator uint() const
   {
      return value;
   }
};



template <class T>
class Singleton
{
public:
   static T& Instance()
   {
      assert (instance);
      return *instance;
   }

   static
   bool isInitialized()
   {
      return instance != nullptr;
   }
protected:
   Singleton()
   {
      assert (instance == nullptr);
      instance = static_cast <T*> (this);
   }

   static T* instance;
};

template <class T>
T* Singleton<T>::instance = nullptr;



} /* namespace mgo */
#endif /* __UTIL_ */

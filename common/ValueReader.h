#ifndef __ValueReader
#define __ValueReader


#include "meta/is_resizeable.h"
#include "util.h"


//*** ValueReader
// Help class to allow assignment like:  array = 1, 6, 9, -2;

namespace mgo {



template <class Array, bool resize = mp::is_resizeable<Array>::value>
class ValueReader             // Array with fixed size
{
public:
   explicit ValueReader (Array &base): array(base), pos(0)  {}

   explicit ValueReader (Array &base, typename Array::const_reference  value): array(base)
   {
      (*this) = value;
   }


   ValueReader&  operator= (typename Array::const_reference  value)
   {
      with (array)   array[pos_] = value;
      pos = 1;
      return *this;
   }
   
   ValueReader&  setValue (typename Array::const_reference  value)
   {
      assert (pos < array.size());
      array[pos++] = value;
      return *this;
   }

   
   operator Array&()            
   { 
      return array; 
   }

   Array& getArray()            
   { 
      return array; 
   }

protected:
   Array   &array;
   size_t   pos;
};




template <class Array>
class ValueReader <Array, true>: public ValueReader <Array, false>
{
public:
   explicit ValueReader (Array &base): ValueReader <Array, false> (base)  {}

   explicit ValueReader (Array &base, typename Array::const_reference  value): 
      ValueReader <Array, false> (base)
   {
      (*this) = value;
   }


   ValueReader&  operator= (typename Array::const_reference  value)
   {
      if (array.empty())  
         array.push_back (value);
      else
         with (array)  array[pos_] = value;
      pos = 1;
      return *this;
   }
   

   ValueReader&  setValue (typename Array::const_reference  value)
   {
      if (pos < array.size())
         array[pos] = value;
      else
         array.push_back (value);
         
      ++pos;
      return *this;
   }
};



template <class Array, class Base>
ValueReader<Array>&  operator, (ValueReader<Array>&  reader, const Base  &value)
{
   reader.setValue (value);
   return reader;
}



template <class Array>
ValueReader<Array>  ValueRead (Array &base)
{
   return  ValueReader<Array> (base);
}


template <class Array>
ValueReader<Array>  ValueRead (Array &base, typename Array::const_reference  value)
{
   return  ValueReader<Array> (base, value);
}


template <class Array>
Array&   getArray (ValueReader<Array>& reader)
{
   return reader;
}



} // namespace mgo
#endif // __ValueReader


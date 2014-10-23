#ifndef  _RANGE_H_
#define  _RANGE_H_

#include "meta/typetraits.h"

namespace mgo {


template <class Iter>
class  Range
{
   static Iter Instance ();
public:
   typedef Iter  iterator;


   typedef decltype(*Instance())  reference;
   typedef typename ::mp::remove_cr<reference>::type   value_type;


   Range (iterator start,  iterator stop): _end(stop), pos(start)   {}

   Range (const Range& other): _end (other._end), pos(other.pos)  {}


   void  reassign (iterator start,  iterator stop)
   {
      pos = start;
      _end = stop;
   }


   template <class Container>
   void  reassign (Container& cont)
   {
      pos = cont.begin();
      _end = cont.end();
   }


   void clear()
   {
      pos = _end;
   }
   
   bool empty() const
   {
      return pos == _end;
   }

   void  pop_front()
   {
      ++pos;
   }

   reference  front() const 
   {
      return *pos;
   }


   iterator  begin() const
   {
      return pos;
   }

   void  begin (iterator  p)
   {
      pos = p;
   }

   iterator  end () const
   {
      return _end;
   }
   
protected:
   iterator  pos, _end;
};



template <class Container>
struct make_range
{
   typedef  Range <typename Container::iterator>  type;
};

template <class Container>
struct make_range <const Container>
{
   typedef  Range <typename Container::const_iterator>  type;
};


template <class Container>
typename make_range<Container>::type  
range_cast (Container& container)
{
   return  typename make_range<Container>::type (container.begin(), container.end());
}



template <class Range, class Filter>
class Subrange: public Range
{
public:
   Subrange (const Range& range, Filter fun): Range(range), filter(fun)  
   {
      while ( !empty() && filter(front()) )
      {
         Range::pop_front();
      }
   }
    
   bool  pop_front()
   {
      do {
         Range::pop_front();
      } while (!empty () && filter (front ()));
   }

private:
   Filter filter;
};




class Chariterator
{
public:
   Chariterator (const char* data): iter(data)  {}


   Chariterator&  operator++ ()
   {
      ++iter;
      return *this;
   }

   char  operator*()
   {
      return *iter;
   }

   Chariterator&  operator+= (int value)
   {
      iter += value;
      return *this;
   }

   operator const char*()
   {
      return iter;
   }
private:
   const char* iter;
};



template <class T>
struct make_iterator
{
   typedef T  type;
};


template <>
struct make_iterator <const char*>
{
   typedef  Chariterator  type;
};

} // namespace mgo
#endif

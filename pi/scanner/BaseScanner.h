#ifndef  _BASESCANNER_H_
#define  _BASESCANNER_H_


#include <stack.h>
#include <sstream>
#include <ErrorInfo.h>



namespace pi {


template <class Iter>
struct TextualIterator: public mgo::make_iterator <Iter>::type
{
   typedef typename mgo::make_iterator <Iter>::type  Base;

   TextualIterator (Iter& iter): Base(iter), line(1), column(1)  {}

   //TextualIterator (const char* iter): Base(iter), line(1), column(1)  {}


   TextualIterator&  operator= (Iter& iter)
   {
      static_cast<Base&> (*this) = iter;
      return *this;
   }

   TextualIterator&  operator++ ()
   {
      char ch = **this;
      if (ch == '\n')
      {
         ++line;
         column = 1;
      }
      if (ch == '\r') {}
      else
         ++column;

      Base::operator++ ();
      return *this;
   }

   unsigned  line;
   unsigned  column;

   //bool  operator== (const TextualIterator<Iter>& lhs, const TextualIterator<Iter>& rhs)
   //{
   //   return  static_cast <const typename TextualIterator::Base&> (lhs) == static_cast<Base&> (rhs);
   //}

   //bool  operator!= (TextualIterator& other)
   //{
   //   bool  Base::operator!= (*this, iter);
   //}


};




template <class Range>
class ScannerData
{
   typedef typename Range::iterator  BaseIterator;
public:

   typedef  TextualIterator<BaseIterator>  iterator;

   ScannerData (const char*  data, const char* end): 
      start_pos (data), end_pos (end), 
      pos (start_pos), error_pos (end_pos)
   {}

   ScannerData (Range& range): 
      start_pos (range.begin()), end_pos (range.end()), 
      pos (start_pos), error_pos (end_pos)
   {}


   void  keepErrorPosition ()
   {
      error_pos = pos;
      //error_info.startRecord ();
   }

   bool  isAtErrorPosition ()
   {
      if (error_pos == pos)  return true;
      if (error_pos == end_pos) 
      { 
         error_pos = pos;
         return true;
      }
      return false;
   }



   BaseIterator   start_pos, end_pos;
   iterator  pos,  error_pos;
   ErrorInfo  error_info;
};



} // namespace pi
#endif 


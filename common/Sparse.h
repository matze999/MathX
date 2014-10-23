#ifndef __SPARSE_H_
#define __SPARSE_H_

#include <vector>
#include <forward_list>
#include "Reference.h"


namespace mgo {



template <class T>
class SelectionList: private std::forward_list<T>
{
   using Super = std::forward_list<T>;
public:
   using  SelectionValue = Reference<value_type>;
   //using  reference = Super::reference;

   template <class ...Args>
   SelectionValue  emplace (Args&&... args)
   {
      emplace_front (args...);
      return  front ();
   }

   template <class ...Args>
   SelectionValue  newSelectionValue (Args&&... args)
   {
      emplace_front (args...);
      return  front ();
   }


   SelectionValue  operator[] (const_reference value)
   {
      auto  pos = find (begin (), end (), value);
      if (pos == end ())
      {
         push_front (value);
         return  front ();
      }
      return  *pos;
   }


   using Super::value_type;
   using Super::reference;
   using Super::const_reference;

   using Super::iterator;
   using Super::const_iterator;

   using Super::begin;
   using Super::end;
   using Super::empty;
};





template <class T>
class SparseBuf
{
protected:
   using  Data = std::forward_list <T>;
   using  Dict = std::vector <T*>;
public:

   using  value_type = typename Data::value_type;
   using  reference = typename Data::reference;
   using  const_reference = typename Data::const_reference;
   using  pointer = T*;


   class iterator;


   SparseBuf () = default;

   explicit SparseBuf (size_t len) : dict (len)
   {
      data.emplace_front ();
      std::fill (dict.begin (), dict.end (), &data.front());
   }

   reference  operator[] (size_t pos)
   {
      data.push_front (*dict[pos]);
      dict[pos] = &data.front();
      return data.front ();
   }

   const_reference  operator[] (size_t pos) const
   {
      return  *dict[pos];
   }


   size_t  size () const
   {
      return  dict.size ();
   }

   bool empty () const
   {
      return  dict.empty ();
   }

   void push_back (const_reference value)
   {
      dict.push_back (getDataPosition (value));
   }

   void push_front (const_reference value)
   {
      dict.push_front (getDataPosition (value));
   }

   template <class ...Args>
   void emplace_back (Args&&...  args)
   {
      value_type  value (args...);
      dict.push_back (getDataPosition (value));
   }


   iterator  begin ()
   {
      return iterator (dict.begin ());
   }

   iterator  end ()
   {
      return iterator (dict.end ());
   }

protected:
   T*  getDataPosition (const_reference  value)
   {
      auto  pos = find (data.begin(), data.end(), value);
      if (pos == data.end ())
      {
         data.push_front (value);
         return  &data.front();
      }
      return  &(*pos);
   }

   Data   data;
   Dict   dict;
};



template <class T>
class SparseBuf<T>::iterator: public Dict::iterator
{
   using  Super = typename Dict::iterator;
public:
   iterator (const Super&  pos): Super (pos) {}

   typename SparseBuf<T>::reference operator* ()
   {
      return  **static_cast <Super&> (*this);
   }

   T* operator-> ()
   {
      return  *static_cast <Super&> (*this);
   }



   friend
   bool operator== (const iterator&lhs, const iterator& rhs)
   {
      return  static_cast <const Super&> (lhs) == static_cast <const Super&> (rhs);
   }

   friend
   bool operator!= (const iterator&lhs, const iterator& rhs)
   {
      return  static_cast <const Super&> (lhs) != static_cast <const Super&> (rhs);
   }
};


} // namespace mgo
#endif 

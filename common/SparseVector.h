#ifndef __COMBOLIST_H_
#define __COMBOLIST_H_

#include "Wrapper.h"
#include <forward_list>



namespace mgo {


template <class T>
class SelectionList
{
public:
   using  value_type = typename std::forward_list<T>::value_type;
   using  reference = typename std::forward_list<T>::reference;
   using  const_reference = typename std::forward_list<T>::const_reference;

   using  iterator = typename std::forward_list<T>::iterator;
   using  const_iterator = typename std::forward_list<T>::const_iterator;

   using  SelectionValue = Reference <value_type>;


   SelectionList (): list (new std::forward_list<T>)  {}

   SelectionList (SelectionList& other): list (other.list)  {}


   template <class ...Args>
   SelectionValue  emplace (Args&&... args)
   {
      list.get().emplace_front (args...);
      return  list.get().front ();
   }

   template <class ...Args>
   SelectionValue  newSelectionValue (Args&&... args)
   {
      return  emplace (args...);
   }


   SelectionValue  operator[] (const_reference value) const
   {
      auto  pos = find (begin (), end (), value);
      if (pos == end ())
      {
         push_front (value);
         return  front ();
      }
      return  *pos;
   }


   iterator begin () 
   {
      return  list.get().begin();
   }

   iterator end () 
   {
      return  list.get().end();
   }

   const_iterator  begin () const
   {
      return  list.get ().begin ();
   }

   const_iterator  end () const
   {
      return  list.get ().end ();
   }

   bool empty() const
   {
      return  list.get().empty();
   }

private:
   WeakReference<std::forward_list<T>>  list;
};




template <class T, class Data = SelectionList<T>>
class SparseVector: public Data
{
   using  Dict = std::vector <typename Data::SelectionValue>;
public:
   using  typename Data::SelectionValue;

   using  typename Data::value_type;
   using  typename Data::reference;
   using  typename Data::const_reference;

   using  iterator = typename Dict::iterator;
   using  const_iterator = typename Dict::const_iterator;


   const_reference  operator[] (size_t pos) const
   {
      return  dict[pos];
   }

   void push_back (SelectionValue item)
   {
      dict.push_back (item);
   }

   void push_front (SelectionValue item)
   {
      dict.push_front (item);
   }


   Data&  SelectionList ()
   {
      return  *this;
   }

   const Data&  SelectionList () const
   {
      return  *this;
   }


   iterator  begin () 
   {
      return dict.begin ();
   }

   iterator  end () 
   {
      return dict.end ();
   }


   const_iterator  begin () const
   {
      return dict.begin ();
   }

   const_iterator  end () const
   {
      return dict.end ();
   }

private:
   using Data::emplace;
   using Data::empty;
   using Data::operator[];

   Dict   dict;
};



} // namespace mgo
#endif 

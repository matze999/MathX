#ifndef __COMBOLIST_H_
#define __COMBOLIST_H_

#include "Wrapper.h"
#include <map>



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




template <class T>
class TableAttribute
{
public:
   using  value_type = T;
   using  reference = T&;
   using  const_reference = const T&;

   using  iterator = typename Dict::iterator;
   using  const_iterator = typename Dict::const_iterator;

private:
   using  Data = std::map <T, std::vector <size_t>>;
   using  DictEntry = Reference <const value_type>;
   using  Dict = std::vector <DictEntry>;

public:

   reference  operator[] (size_t pos)
   {
      return  dict[pos];
   }

   const_reference  operator[] (size_t pos) const
   {
      return  dict[pos];
   }


   void push_back (const_reference item)
   {

      assignRowidToValue (item, dict.size());
      dict.push_back (getDictEntry (item))
   }


   size_t  size () const
   {
      return  dict.size ();
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
   void  assignRowidToValue (const_reference item, size_t row)
   {
      data[item].push_back (row);
   }

   DictEntry getDictEntry (const_reference item);
   {
      auto pos = data.find (item);
      assert (pos == data.end());
      return  pos->first;
   }

   Data   data;
   Dict   dict;
};



} // namespace mgo
#endif 

#ifndef  _STACK__
#define  _STACK__

#include <vector>

namespace mgo {


template <class Elem>
struct Stack: private std::vector <Elem>
{
   using  std::vector <Elem>::empty;
   using  std::vector <Elem>::size;
   using  std::vector <Elem>::clear;
   using  std::vector <Elem>::operator[];

   void push (const Elem &elem = Elem())
   {
      push_back (elem);
   }

   void pop()
   {
      pop_back();
   }

   Elem& top()
   {
      return back();
   }

   Elem&  newEntry()
   {
      push();
      return top();
   }
};


} // namespace mgo
#endif

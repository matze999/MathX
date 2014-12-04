#ifndef  _MGO_INLIST_H_
#define  _MGO_INLIST_H_


/*
Define the macro IN which act like an binary operator 

   a IN {3, 6, 7} - check if a is the set {2, 6, 7}
   a IN (2, 8)    - check if a is in the interval (2, 9)
   a IN (9)       - check if a is in the interval (0, 9)
*/

class  _inlist 
{
   initializer_list<int>  list;
   int lb, ub;
public:

   _inlist (initializer_list<int> list): list(list)  {}

   _inlist (int lb, int ub): lb(lb), ub(ub)  {}

   _inlist (int ub): _inlist(0, ub)  {}


   bool contains (int value) const
   {
      if (list.size () == 0)
         return  lb <= value  &&  value <= ub;
      else for (int elem : list)
      {
         if (elem == value)  return true;
      }
      return false;
   }
};


inline
bool operator| (int value, const _inlist& list)
{
   return list.contains (value);
}

inline
bool operator|| (int value, const _inlist& list)
{
   return !list.contains (value);
}

#define IN  | _inlist
#define NOT_IN  || _inlist

#endif

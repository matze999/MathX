#ifndef  _TRIVAL_H_
#define  _TRIVAL_H_

#include "util.h"



class trival
{
public:
   trival () = default;
   trival (bool val): value (val)  {}
   trival (int val): value (val>2? 2: val)  {}


   trival&  operator= (const trival&) = default;

   trival&  operator= (bool val)
   {
      value = val;
      return *this;
   }


   friend  bool  operator== (trival lhs, trival rhs)
   {
      return lhs.value == rhs.value;
   }

   friend  bool  operator== (trival lhs, bool rhs)
   {
      return lhs.value == byte (rhs);
   }

   friend  bool  operator== (bool lhs, trival rhs)
   {
      return rhs.value == byte (lhs);
   }


   operator bool () const
   {
      return value == 1;
   }

   trival  operator! () const
   {
      return value == 0? 1:
             value == 1? 0: 2;
   }

private:
   byte  value = 0;
};


const trival undef (2);




inline
bool  operator!= (trival lhs, trival rhs)
{
   return  !(lhs == rhs);
}

inline
bool  operator!= (trival lhs, bool rhs)
{
   return  !(lhs == rhs);
}

inline
bool  operator!= (bool lhs, trival rhs)
{
   return  !(lhs == rhs);
}


inline
trival  operator|| (trival lhs, trival rhs)
{
   return  lhs == true? true:
      rhs == true? true:
      lhs == undef? undef: rhs;
}


inline
trival  operator&& (trival lhs, trival rhs)
{
   return  lhs == false? false:
      rhs == false? false:
      lhs == undef? undef: rhs;
}

#endif

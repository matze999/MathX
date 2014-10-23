#ifndef  _MutableType_H_
#define  _MutableType_H_

#include "meta/typetraits.h"


namespace mgo {


template <class U, class V = void>
class MutableType
{
public:
   typedef  U  first_type;
   typedef  V  second_type;

   MutableType(): active_type(0) {}

   MutableType (first_type& item): active_type(1)
   {
      first = new first_type (item);
   }

   MutableType (second_type& item): active_type(2)
   {
      second = new second_type (item);
   }

   ~MutableType()
   {
      if (active_type == 1)
         delete first;
      else if (active_type == 2)
         delete second;
   }


   operator first_type&()
   {
      assert (active_type == 1);
      return *first;
   }

   operator second_type&()
   {
      assert (active_type == 2);
      return *second;
   }


   operator const first_type&() const
   {
      assert (active_type == 1);
      return *first;
   }

   operator const second_type&() const
   {
      assert (active_type == 2);
      return *second;
   }

   first_type&  FirstType()
   {
      assert (active_type == 1);
      return *first;
   }

   second_type&  SecondType()
   {
      assert (active_type == 2);
      return *second;
   }

   const first_type&  FirstType() const
   {
      assert (active_type == 1);
      return *first;
   }

   const second_type&  SecondType() const
   {
      assert (active_type == 2);
      return *second;
   }


   MutableType&  operator= (const first_type& item)
   {
      if (active_type == 1)
      {
         *first = item;
      }
      else
      {
         if (active_type == 2)  delete second;
         active_type = 1;
         first = new first_type (item);
      }
      return *this;
   }


   MutableType&  operator= (const second_type& item)
   {
      if (active_type == 2)
      {
         *second = item;
      }
      else
      {
         if (active_type == 1)  delete first;
         active_type = 2;
         second = new second_type (item);
      }
      return *this;
   }


   MutableType& Super()
   {
      return *this;
   }

   const MutableType& Super() const
   {
      return *this;
   }

   bool isFirstType()
   {
      return  active_type == 1;
   }

protected:
   union 
   {
      U*  first;
      V*  second;
   };
   char  active_type;
};


#define  applyMethod(mt, method)  \
   mt.isFirstType()?  mt.FirstType(). method: \
                      mt.SecondType(). method;



template <class T>
class MutablePodType 
{
   typedef  T  value_type;
   typedef  typename mp::add_reference<T>::type  reference;
   typedef  typename mp::make_const<reference>::type  const_reference;
public:
   MutablePodType (reference val): is_const(false)  
   {
      data = &val
   }
   MutablePodType (const_reference val): is_const(true)
   {
      data = const_cast <reference> (val);
   }

   operator reference()
   {
      assert (!is_const);
      return data;
   }

   operator const_reference() const
   {
      return data;
   }


   const_reference  ConstValue() const
   {
      return data;
   }

   reference  operator() ()
   {
      return operator reference (data);
   }

private:
   value_type  data;
   bool  is_const;
};



template <class T>
class MutableClassType: public T
{
   typedef  T  value_type;
public:
   MutableClassType (value_type& val): T(val), is_const(false)  {}
   MutableClassType (const value_type& val): T(val), is_const(true)  {}

   operator value_type&()
   {
      assert (!is_const);
      return *this;
   }

   operator const value_type&() const
   {
      return *this;
   }


   const value_type&  ConstValue() const
   {
      return *this;
   }

   value_type&  assertNonConst()
   {
      assert (!is_const);
      return *this;
   }
private:
   bool  is_const;
};


template <class T>
class MutableType <T, void>:
   mp::if_<mp::or <mp::is_pod<T>, mp::is_reference<T>>::value, 
      MutablePodType<T>,
      MutableClassType<T>>::type
{
   typedef  T  value_type;
   typedef  typename mp::add_reference<T>::type  reference;
   typedef  typename mp::make_const<reference>::type  const_reference;

   typedef  typename mp::if_<mp::or <mp::is_pod<T>, mp::is_reference<T>>::value, 
      MutablePodType<T>,
      MutableClassType<T>>::type  Base;

public:
   MutableType (reference val): Base(val)  {} 
   MutableType (const_reference val): Base(val)  {} 
};

} // namespace mgo
#endif

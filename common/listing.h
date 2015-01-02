#ifndef __LISTING_
#define __LISTING_

#include <util.h>

/* How To Use

struct  Color: public mgo::Listing <Color, const char*>  
{
   static const Item red, blue, green;
};

const mgo::Listing <Color, const char*>  Color::red("red"), Color::blue("blue"), Color::green("green");

/***

namespace Farbe
{
   class Item{};
   const mgo::Listing <Item, const char*>  red("red"), blue("blue"), green("green");
}

/***

class Autos: public mgo::Listing <Autos, const char*>  {};

const Autos::Item  red("red"), blue("blue"), green("green");

*/


namespace mgo {


template 
<
   class  Base, 
   class  Target,
   int    capacity = 20
>
class Listing
{
public:
   typedef Listing Item;

   unsigned id;

   Listing (): id (Listing::npos)  {}

   Listing (const Target &target)
   {
      assert0 (length < capacity);

      id = length++;
      data[id] = target;
   }

   //Listing (const char* target)
   //{
   //   assert0 (length < capacity);

   //   id = length++;
   //   data[id] = target;
   //}


   const Target&  operator* () const      { return  data[id]; }
   const Target*  operator-> () const     { return  data + id; }

   operator unsigned() const              { return  id + 1000; }
   operator bool() const                  { return  id != npos; }

   friend
   bool  operator== (Listing  lhs,  Listing  rhs)
   {
      return  lhs.id == rhs.id;
   }

   friend
   bool  operator!= (Listing  lhs,  Listing  rhs)
   {
      return  lhs.id != rhs.id;
   }

// static member:
   typedef  Target*  iterator;

   static  size_t size()               { return length; }

   static  iterator  begin()           { return  data; }
   static  iterator  end()             { return  data + length; }


   struct  Instances_t
   {
      typedef  Target*  iterator;

      static  size_t size()            { return length; }

      static  iterator  begin()        { return  data; }
      static  iterator  end()          { return  data + length; }

   };

   static Instances_t  Instances;

protected:
   static const unsigned  npos = -1;

   static Target    data[capacity];
   static unsigned  length;
};



template <class Base, class Target, int capacity>
Target  Listing <Base, Target, capacity>::data[capacity];


template <class Base, class Target, int capacity>
size_t  Listing <Base, Target, capacity>::length = 0;



#define NamedVar(x)  x(#x)



} // namespace mgo
#endif
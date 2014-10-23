#ifndef __META_WHILE_
#define __META_WHILE_

/*** 

>>> WHILE example

template <int VAL>
struct condition
{
   enum { is_true = VAL < 1024,
          value = VAL };

   typedef  typename
   IF is_true THEN
      condition <2*value>
   ELSE
      void
   END::type  next;
};

struct code
{
   template <class T>
   static void execute()
   {
      cout << "value: " << T::value << endl;
   }
};

WHILE condition<1> BEGIN code END::execute();

*/

#define  BEGIN        ,
#define  FOR          _FOR <
#define  WHILE        _WHILE <


namespace mp {

struct void_ 
{
   typedef void_ type;

   template <class T>
   static void execute()  {}

   static void execute()  {}
};




//*** meta for_

template <int begin, int end, int step = (begin < end: 1, -1)>
struct for_
{
   template <class code>
   static
   void execute()
   {
      code::template execute<value> ();
      for_ <begin + step, end, step>::execute <code> ();
   }
};

template <int end, int step>
struct for_ <end, end, step>: public void_  {};



//*** meta while_

template <class condition, bool = condition::value>
struct while_
{   
   template <class code>
   static void  execute()
   {
      code::template execute <condition> ();
      while_ <typename condition::next>::execute <code> ();
   }
};

template <class condition>
struct while_ <condition, false>: public void_  {};


} // namespace mp
#endif


#include <Wrapper.h>

using  namespace mgo;



struct  TestClass
{
   int  value;

   double  getDouble (double d)
   {
      return d;
   }

   int  ConstMethod() const
   {
      return 4;
   }
};




void  testWrapperReference ()
{
   int val = 4;

   Wrapper <int, Assoc::Reference>  w1 = val;
   assert (w1 == val);

   w1 = 12;
   assert (val == 12);

   int val2 = rand();
   w1.reassign (val2);

   
   Wrapper <char>  w2;
   assert (!w2.is_valid());
   char ch = 34;
   w2.reassign (ch);
   assert (w2.is_valid());

   TestClass  t;

   Reference <TestClass> w3 = t;
}


void test_WeakReference ()
{
   using  WeakReference = Wrapper <string, Assoc::WeakReference>;

   string val = "Hallo";

   WeakReference  ref1;
   assert (!ref1.is_valid());

   WeakReference ref2 = val;
   assert (ref2 == string ("Hallo"));

   ref2 = "Welt";
   assert (ref2 == string ("Welt"));
   assert (val == string ("Welt"));

   ref2.detach ();
   assert (ref2 == string ("Welt"));

   ref2 = "World";
   assert (ref2 == string ("World"));
   assert (val == string ("Welt"));

   ref2.reassign (val);
   assert (ref2 == string ("Welt"));


   ref2.reassign (new string ("Test"));
   assert (ref2 == string ("Test"));
   assert (val == "Welt");


   const Reference <string> cref;

   WeakReference ref4 = ref2;
   assert (ref4 == string ("Test"));


   WeakReference ref5 = new string ("Test");
   assert (ref5 == string ("Test"));

}

void  wrapper_test()
{
   testWrapperReference();
   test_WeakReference();
}


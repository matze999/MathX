
#include <Reference.h>
#include <vector>


void  test_Reference ()
{
   using  namespace mgo;


   std::vector<int> vec;

   Reference <std::vector<int>> ref = vec;
   ref.push_back (4);
   assert (vec.size() == 1);

   ref.emplace_back (6);
   assert (vec.size() == 2);

   vec.push_back (2);
   assert (ref[2] == 2);
   assert (ref.size() == 3);
   assert (!ref.empty ());


   ref.clear ();
   assert (vec.empty ());
}



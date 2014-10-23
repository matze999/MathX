
#include <SparseVector.h>
#include <utility>
#include <algorithm>
#include "VectorUtils.h"

using namespace mp;
using namespace mgo;



struct MySelection: public SelectionList <string>
{
   SelectionValue  red, green, blue;

   MySelection ():
      red (newSelectionValue ("red")),
      green (newSelectionValue ("green")),
      blue (newSelectionValue ("blue"))
   {}
};



using MyVector1 = SparseVector <string, MySelection>;


struct MyVector2: public SparseVector <string>
{
   SelectionValue  one, two, three;

   MyVector2 ():
      one (newSelectionValue ("one")),
      two (newSelectionValue ("two")),
      three (newSelectionValue ("three"))  {}
};







void  test_SparseVector ()
{
   MySelection  selection;

   cout << "Selection: " << writeAsList (selection) << endl;

   MySelection  selection2 = selection;

   selection2.newSelectionValue ("black");
   //assert (selection.size () == 4);




   SparseVector <string> list;

   auto red = list.newSelectionValue ("red"),
      blue = list.newSelectionValue ("blue"),
      green = list.newSelectionValue ("green");

   list.push_back (red);
   list.push_back (green);
   list.push_back (blue);
   list.push_back (blue);
   list.push_back (red);

   cout << "combo list: " << writeAsList (list) << endl;

   cout << "test: " << red << endl;


   MyVector1  vec2;
   vec2.push_back (vec2.blue);
   vec2.push_back (vec2.red);
   vec2.push_back (vec2.green);

   vec2.push_back (selection.red);
   vec2.push_back (selection.blue);

   cout << "Combo: " << writeAsList (vec2) << endl;
   cout << "SelectionList: " << writeAsList (vec2.SelectionList ()) << endl;



   MyVector2  vec1;
   vec1.push_back (vec1.one);
   vec1.push_back (vec1.three);
   vec1.push_back (vec1.two);

   cout << "Combo: " << writeAsList (vec1) << endl;
   cout << "SelectionList: " << writeAsList (vec1.SelectionList ()) << endl;


}

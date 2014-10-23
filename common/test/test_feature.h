
#include <iostream>

using  namespace mgo;

template <class T>
struct enableNumericTypes: public promote<char>  {};

class ASD{};

template <class T>
struct change_type  {};

template <class T>
struct enableNumericTypes <change_type<T>>: public 
   enable_if <is_arithmetic<T>::value, promote <int>>::type
{
};




struct OpClass  
{
   OpClass& operator[] (int)
   {
      return *this;
   }

   OpClass& operator[] (OpClass)
   {
      return *this;
   }
};

OpClass& operator& (OpClass& inst)
{
   return inst;
}


OpClass& operator* (OpClass& inst)
{
   return inst;
}


OpClass& operator| (OpClass& inst, OpClass&)
{
   return inst;
}

OpClass& operator* (OpClass& inst, OpClass&)
{
   return inst;
}

OpClass& operator, (OpClass& inst, OpClass&)
{
   return inst;
}


struct Integer
{
   Integer()  {}
   Integer (int val): value(val)  {}

   operator int&()      { return value; }
   operator int() const { return value; }
   int value;
};

int operator, (Integer, int)
{
   return 4;
}



//
//template <class T, class Enable = void> 
//struct AB { std::string text; };
//
//template <class T>
//struct AB<T, typename enable_if<is_integral<T>::value>::type> { int val; };
//template <class T>
//struct AB<T, typename enable_if<is_void<T>::value>::type> { bool bval; };


void test_Features()
{

   //::mp::write (cout, ::mp::remove_multiple_of<3, List>());
   //::mp::write (cout, ::mp::SieveOfEratosthenes<1000>());




   //AB<int>  ab1;
   //AB<String> ab2;
   //AB<void> ab3;

   //ab1.val;
   //ab2.text;
   //ab3.bval;

   OpClass u, v;

   u |& v;

   v = v*******u;

   u[v, u];
   u[Integer(2), 4];
}




#include <iostream>

using namespace std;


template <class T> 
struct  TypeHolder
{
   TypeHolder (const T& val): value (val)
   {}

   void execute ()
   {
      cout << "passed rvalue" << endl;
   }

   T  value;
};


template <class T> 
struct  TypeHolder <const T&>
{
   TypeHolder (const T& val): value (val)
   {}

   void execute ()
   {
      cout << "passed const lvalue" << endl;
   }

   T  value;
};


template <class T> 
struct  TypeHolder <T&>
{
   TypeHolder (const T& val): value (val)
   {}

   void execute ()
   {
      cout << "passed lvalue" << endl;
   }

   T  value;
};


void TypeFunc ()    
{
   cout << "passed no args" << endl;
}

template <class T>
void TypeFunc (T&& val)
{
   cout << "passed 1 arg" << endl;
   TypeHolder<T> (val).execute ();
}



template <class T1, class T2>
void TypeFunc (T1&& val1, T2&& val2)  
{
   cout << "passed 2 args" << endl;
   TypeHolder<T1> (val1).execute ();
   TypeHolder<T2> (val2).execute ();
}





template <class T>
TypeHolder<T>  holdType (T&& value)
{
   return  TypeHolder<T> (value);
}

template <class ...Args>
TypeHolder<Args...>  holdTypes (Args&&... value)
{
   return  TypeHolder<Args...> (value...);
}

template <class ...Args>
TypeHolder<Args...>  holdTypes2 (Args... value)
{
   return  TypeHolder<Args...> (value...);
}



template <class ...Args>
void  callTypeFunc (Args&&... value)
{
   return  TypeFunc (value...);
}




template <class T>
void InFunc (const T& val)
{
   cout << "InFunc <const T&>" << endl;
}

template <class T>
void InFunc (T& val)
{
   cout << "InFunc <T&>" << endl;
}


template <class T>
void InFunc (const T&& val)
{
   cout << "InFunc <const T&&>" << endl;
}



template <class T>
void OutFunc (const T& val)
{
   cout << "OutFunc <const T&>" << endl;
   cout << "direct call: "; 
   InFunc (val);
   cout << "forward call: "; 
   InFunc (forward<const T&>(val));
   InFunc (forward<const T&&>(val));
   cout << "move call: "; 
   InFunc (move(val));
}

template <class T>
void OutFunc (T& val)
{
   cout << "OutFunc <T&>" << endl;
   cout << "direct call: "; 
   InFunc (val);
   cout << "forward call: "; 
   InFunc (forward<T&>(val));
   cout << "move call: "; 
   InFunc (move(val));
}

template <class T>
void OutFunc (const T&& val)
{
   cout << "OutFunc <const T&&>" << endl;
   cout << "direct call: "; 
   InFunc (val);
   cout << "forward call: "; 
   InFunc (forward<const T&&>(val));
   InFunc (forward<const T&>(val));
   cout << "move call: "; 
   InFunc (move(val));
}




void testRValue ()
{


   int val = 6;
   const int cval = 9;


   OutFunc (val);
   OutFunc (cval);
   OutFunc (4);

   auto hold1 =  holdType (5);
   auto hold2 = holdType (val);
   auto hold3 = holdTypes (cval);
   auto hold4 = holdTypes2 (cval);

   hold3.execute ();
   hold4.execute ();


   callTypeFunc ();
   callTypeFunc (val);
   callTypeFunc (val, cval);
}

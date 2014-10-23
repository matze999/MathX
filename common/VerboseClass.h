#ifndef  _VERBOSE_CLASS__
#define  _VERBOSE_CLASS__



struct VerboseClass
{
   VerboseClass()  
   {
      cout << "run default constructor" << endl;
   }

   VerboseClass (const VerboseClass&)  
   {
      cout << "run copy constructor" << endl;
   }

   VerboseClass (VerboseClass&&)  
   {
      cout << "run move constructor" << endl;
   }

   template <class T>
   VerboseClass (T)  
   {
      cout << "run user defined constructor" << endl;
   }


   ~VerboseClass()
   {
      cout << "run destructor" << endl;
   }


   VerboseClass& operator= (const VerboseClass&)
   {
      cout << "run default copy operator" << endl;
      return *this;
   }

   template <class T>
   VerboseClass& operator= (T)
   {
      cout << "run user copy operator" << endl;
      return *this;
   }
};
   
#endif

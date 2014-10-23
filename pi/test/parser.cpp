#include  <iostream>
#include "../Expression.h"

// Curiously recurring template pattern (CRTP)
// the Barton and Nackman Trick

/*
template<typename T>
class IsClassT {
  private:
    typedef char One;
    typedef struct { char a[2]; } Two;
    template<typename C> static One test(int C::*);
    // Will be chosen if T is anything except a class.
    template<typename C> static Two test(...);
  public:
    enum { Yes = sizeof(IsClassT<T>::test<T>(0)) == 1 };
    enum { No = !Yes };
};

When IsClassT<int>::Yes is evaluated, 0 cannot be converted to int int::* because int is not a class, so it can't have a member pointer. If SFINAE didn't exist, then you would get a compiler error, something like '0 cannot be converted to member pointer for non-class type int'. Instead, it just uses the ... form which returns Two, and thus evalutates to false, int is not a class type.
*/



using namespace pi;
using namespace std;





template <class Parser, class SkipParser>
void  testParser (const string  &data, Parser &parser, SkipParser &skip_parser)
{
   string  result;
   
   parser.toString (result, 10);
   cout << "using parser: " << result << endl;
   cout << "input string: \"" << data << "\"\n" << endl;


   if (!parse (data, parser, skip_parser, result))
   {
      cout << "\n  Error parsing: '" << data << "'\n"
           << "parser stack: " << endl;

      with (std::string &entry, parse.error_info.stack)
      {
         cout << "  ("<< pos_ << ") " << entry << endl;
      }
      return;
   }


   if (data != result)
   {
      cout << "\n  Warning: The input end is not achieved! \n  input: '" << data 
           << "'\n   read: '" << result << "'\n" << endl;
   }


}



struct sum_f
{
   sum_f (int &val): value(val), is_first(true)  {}


   void operator() (int val)
   {

   }

   char  op;
   bool  is_first;
   int &value;
};



template <class signatur, class Command = std::string>
class Operator;


template <class Base, class Command>
class Operator <Base(Base&, Base&), Command>: 
   public function_traits <Base(Base&, Base&)>
{
public:
   Operator (result_type  &res): result(res), read_first(true)  {}

   void  operator()  (arg)

private:
   bool   read_first

   result_type  &result;
   arg1_type  arg1;
   arg2_type  arg2;
   Command    command;
};


int main()
{

   Char  sd = char_("sd");
   Char  hallo("n");



   testParser ("ansd", 
               char_("asd") & char_  &  *sd,
               blank);

   testParser ("a       n s d", char_("a") & char_("n") | char_("a") &  sd,
               blank);


   testParser ("a, b, c, d, e", char_ % ",", blank);



   Rule <int, decltype(blank), sum_f>  sum = int_ & char_("+-") & int_;

   cout << "Tests finished!" << endl;
   getchar();
   return 0;
}


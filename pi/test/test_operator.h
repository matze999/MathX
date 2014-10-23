#include "test_base.h"
#include "../Operator.h"
#include <functional>

void test_Option()
{
   Rule <String>  parser;

   parser = "name:" >> (token("opt1"), token("opt2"), token("opt3"));

   assertParser (parser, "name:opt1opt2opt3", "opt1opt2opt3");
   assertParser (parser, "name:opt3opt2opt1", "opt3opt2opt1");
   assertParser (parser, "name:opt3opt1", "opt3opt1");
   assertParser (parser, "name:opt2", "opt2");
   assertParser (parser, "name:opt3", "opt3");

   //assertParserFailure (parser, "name:", "");
}



void test_Operator ()
{
   Rule<int>  sum;

   Operator<int (int, int)>  plus ("+", [](int a, int b) { return a + b; });
   Operator<int>  minus ("-", [](int a, int b) { return a - b; });
   

   // pi::_::and_p <pi::_::double_p, decltype(plus)> parser0 (double_, plus);
   sum =  double_ >> -((plus | minus) >> double_);


   int result;
   sum.parse ("4+3", result);
   assert (result == 7);

   sum.parse ("4-3", result);
   assert (result == 1);

   sum.parse ("4", result);
   assert (result == 4);


   auto  parser = double_ % (plus | minus);

   parser.parse ("4+3", result);
   assert (result == 7);

   parser.parse ("4-3", result);
   assert (result == 1);

   parser.parse ("4", result);
   assert (result == 4);

   Rule<int>  plusminus;

   plusminus = (double_ >> plus >> double_) >> minus >> double_;

   assertParser (plusminus, Scanner<> ("4+3-2"), 5);

}



void test_Calculator ()
{
   Rule <double>  sum, product, term;


   Operator<double> plus {"+", std::plus <double> {}};
   Operator<double> minus {"-", std::minus <double> {}};
   Operator<double> multiply {"*", std::multiplies <double> {}};
   Operator<double> divide {"/", std::divides <double> {}};


   sum = product % (plus | minus);

   product = term % (multiply | divide);

   term = double_
        | "(" >> sum >> ")";

   assertParser (sum, Scanner<>("4+5*2"), 14);
   assertParser (sum, Scanner<>("(4+5)*2"), 18);
}


void test_Calculator2 ()
{
   Rule <double>  sum, product, term;

   using op = pi::op < double > ;
   UnaryOperator<double>  inc ("++", [](double val) { return ++val; });
   UnaryOperator<double>  dec ("--", [](double val) { return --val; });


   sum = product % (op::plus | op::minus);

   product = term % (op::multiply | op::divide);

   term = (inc | dec) >> double_
        | double_
        | "(" >> sum >> ")";

   assertParser (sum, Scanner<>("4+5*2"), 14);
   assertParser (sum, Scanner<>("(4+5)*2"), 18);

   assertParser (sum, Scanner<>("(4+5)*--2"), 9);

}


/*
void test_Function ()
{
   Function <double (double)>  sin ("sin", sinf, "(", ")"), cos ("cos", cosf);

   term = sin (double_) | cos (double_)
      cos

}
*/
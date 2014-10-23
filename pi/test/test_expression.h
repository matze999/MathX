#include <boost/lambda/lambda.hpp>
#include <util.h>



void test_Switch()
{
   String  sval = "undef";
   int     ival = 0;
   bool    bval = false;


   auto expr = switch_(word)
      [
         case_("int")    [int_(ival)],
         case_("string") [word(sval)],
         case_("bool")   [bool_(bval)]
      ];

   Rule0 <std::string> parser;
   parser = expr% ",";
   parser.toString (cout);

   const char* input = "bool true, string hallo, int 4";

   Skip  scanner (input);
   if (!parse (expr % ",", scanner, fn::no_action()))
      cout <<  scanner.getErrorInfo();


   Rule0 <std::string> sw;
   sw = switch_ (int_)
      [
         case_ (4)[var ("vier")],
         case_ (3)[var ("drei")],
         case_ (1)[var ("eins")],
         case_ (2)[var ("zwei")],
         default_[var ("Big_Number")]
      ];

   assertParser (sw % char_ (','), "1,2,3,4,5", "eins,zwei,drei,vier,Big_Number");
}
//


void test_Functor()
{
    using namespace boost::lambda;
   //using namespace boost::phoenix::arg_names;
   String  name;
   int result, val = 4;

   auto pars1 = word [var(name) = _1];
   auto pars2 = word [var(val)];

   parse (pars1, "hallo", fn::no_action());
   parse (pars2, "hallo", result);


   auto pars3 = word [[&](String& val){ return name = val; }];
   auto pars4 = word [[](...){ return 10;}];

   auto fun = [](...){ return 10;};

   //cout << fun;

   parse (pars3, "other", fn::no_action());
   parse (pars4, "other", result);

}


struct intstr
{
   String name;
   int    value;

   void operator= (int val)      { value = val; }
   void operator= (String val)   { name = val; }
};


void test_Switch2()
{
   String  sval;
   int     ival = 5;
   bool    bval = false;

   //TRC_LEVEL = TL_DEBUG;

   auto expr = switch_(word)
      [
         case_("one")      [var("eins")],
         case_("two")      [pi::ref(ival)],
         case_("three")    [var("drei")]
      ];


   auto parser = expr % char_ (',');

   parser.toString (cout, WRITEMODE::MINIMAL);
   cout << endl;
   parser.toString (cout, WRITEMODE::COMPACT);
   cout << endl;
   assertParser (parser, "three,one,two", "drei,eins,5");
}




void  test_Map()
{
	//pi::Map <Char, int> mapping;
	//mapping.add ('1', 1) ('2', 2) ('3', 3) ('4', 4).default_ (0);

	//pi::Rule <string> parser;
	//parser = mapping % ", ";

	//assertParser (parser, "1, 2, 3, a", "1230");
}
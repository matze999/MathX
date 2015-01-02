#include <util.h>

using namespace mgo;


template <class Scanner>
void testAndOr()
{

   // sequence
   auto seq_abc = char_('a') >> char_('b') >> char_('c');

   auto seq_ac = char_('a') >> "b" >> char_('c');

   // alternative
   auto or_abc = char_('a') | char_('b') | char_('c');
   auto rep = *or_abc;


   assertParser (seq_abc, Scanner("abc"), "abc");
   assertParser (seq_ac, Scanner("abc"), "ac");

   assertParser (rep, Scanner("abc"), "abc");
   assertParser (rep, Scanner("aaaaabbccdd"), "aaaaabbcc");

   assertParserFailure (seq_abc, Scanner("abd"), String());
   assertParserFailure (seq_abc, Scanner("acc"), String());
   assertParserFailure (or_abc >> rep, Scanner("ddd"), String());
}



void testOperatorsSkip()
{
   //TRC_LEVEL = 4;
   const char*  abc = "\n a  b\t  c\n\n";
   // sequence
   Rule0 <String, Skip>  seq_abc;

   seq_abc = char_('a') >> char_('b') >> char_('c');

   // alternative
   Rule0 <String, Skip>  or_abc;
	or_abc.Name ("or abc");
   Rule0 <String, Skip>  rep = *or_abc;

   std::string result;
   Skip scanner ("\n a  b\t  x\n\n");
   if (!seq_abc.parse (scanner, result))
      cout << scanner.getErrorInfo ();
   

   or_abc = char_('a') | char_('b') | char_('c') | "d";

   parse_assert <Skip> (seq_abc, abc, "abc");
   parse_assert <Skip> (rep, abc, "abc");
   parse_assert <Skip> (rep, "abcdaddbdddc", "abcabc");


   Rule0 <String, Skip> rep2 = *(char_('a') | char_('b') | char_('c'));

   parse_assert <Skip> (rep2, abc, "abc");
   parse_assert <Skip> (+char_("abc"), abc, "abc");
}


void testMinus()
{
   auto  digit = ch::digit / "0";

   assertParser (+digit, "12345", "12345");
   assertParser (+digit, "123401234", "1234");
   assertParserFailure (+digit, "a1234", String());
}


void test_ReferenceParser()
{
   std::string  my_string;
   auto  parser2 = (*char_)(my_string);


   parser2.parse ("12345");
   assert (my_string == "12345");

   auto  term = *(char_ >> pi::ignore(char_));
   auto  parser3 = term (my_string);
   my_string.clear();

   parser3.parse ("12345678");
   assert (my_string == "1357");
}



TEST (NumberTest, Consolidation)
{
   double result;
   parse (number, "2", result);
   ASSERT_DOUBLE_EQ (2, result);

   int  ival;
   parse (number, "2", ival);
   ASSERT_EQ (2, result);

   parse (number, "2.1", result);
   ASSERT_DOUBLE_EQ (2.1, result);

}

void test_Simple()
{
   testAndOr <pi::Scanner<>> ();
   testAndOr <::SkipScanner> ();

   testOperatorsSkip();

   testMinus();

   test_ReferenceParser();
}



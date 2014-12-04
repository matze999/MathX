#ifndef  __TEST_PARSER_RULE_
#define  __TEST_PARSER_RULE_


#include <gtest++.h>
#include <pi/Parser.h>


using namespace pi;




TEST (RuleTest, testStringRule)
{
   Rule0 <std::string> rule1;
   std::string result;

   rule1 = "<" >> char_ >> ">";
   rule1.parse ("<4>", result);
   ASSERT_EQ ("4", result);

   Rule0 <std::string> rule2 = "<" >> ch::alphanum >> ">";
   result.clear ();
   rule2.parse ("<4>", result);
   ASSERT_EQ ("4", result);

   Rule <std::string> rule3;
   rule3 = "<" >> ch::digit >> ">";
   rule3.parse ("<4>", result); // the parsed string is append to the result string
   ASSERT_EQ ("44", result);


   Rule <std::string>  rule4 {"hallo"};
   rule4 = "<" >> ch::alphanum >> ">";
   result.clear ();
   rule4.parse ("<4>", result);
   ASSERT_EQ ("hallo4", result);
}


TEST (RuleTest, Rule0_NoAttribute)
{
   std::string  result = ">";
   Rule0<> rule = "<" >> word(result) >> ">";

   rule.parse ("<hallo>");
   ASSERT_EQ (">hallo", result);


   Rule0<std::string>  rule2 = char_ >> rule >> char_;
   std::string result2;
   rule2.parse ("<<hallo>>", result2);
   ASSERT_EQ ("<>", result2);
}



TEST (RuleTest, Rule0_NoAttribute_SkipScanner)
{
   std::string  result = ">";
   Rule0 <WhiteSpace> rule = "<" >> word(result) >> ">";

   rule.parse (SkipScanner<WhiteSpace>("< hallo \n>"));
   ASSERT_EQ (">hallo", result);

   result.clear ();
   rule.parse ("< hallo \n>");
   ASSERT_EQ ("hallo", result);


   Rule0 <std::string, WhiteSpace>  rule2 = char_ >> rule >> char_;
   std::string result2;
   rule2.parse (SkipScanner<WhiteSpace> ("< \t <hallo>\n>"), result2);
   ASSERT_EQ ("<>", result2);
}

#endif


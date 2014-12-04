#ifndef  __TEST_PARSER_CLASS_PARSER_
#define  __TEST_PARSER_CLASS_PARSER_


#include <gtest/gtest.h>
#include <pi/parser.h>
#include <pi/expression/ClassParser.h>


using namespace pi;

class MyRange: public ClassParser < MyRange >
{
public:
   int lb, ub;

   template <class Scanner>
   bool  parseImp (Scanner& scanner)
   {
      auto parser =  "[" >> int_(lb) >> "," >> int_(ub) >> "]";
      return parser.parse (scanner);
   }
};

using RangeVector = std::vector < MyRange > ;


TEST (ClassParserTest, SimpleParserCall)
{
   MyRange  range;

   range.parse ("[2,5]");

   ASSERT_EQ (2, range.lb);
   ASSERT_EQ (5, range.ub);
}


TEST (ClassParserTest, UsageInsideVector)
{
   Rule <RangeVector>  rule;
   MyRange  range;
   RangeVector  result;

   rule = range % ": ";

   rule.parse ("[1,2]: [-3,-1]", result);

   ASSERT_EQ (2, result.size());
   ASSERT_EQ (1, result[0].lb);
   ASSERT_EQ (-1, result[1].ub);
}



struct Accuracy
{
   int mantisse, decimals;
};


struct AccuracyParser: public Accuracy, public ClassParser < AccuracyParser >
{
   template <class Scanner>
   bool  parseImp (Scanner& scanner)
   {
      decimals = 0;
      auto parser =  "#" >> int_(mantisse) >> -("." >> int_(decimals));
      return parser.parse (scanner);
   }
};


TEST (ClassParserTest, testInheritance)
{
   AccuracyParser  accuracy_parser;

   accuracy_parser.parse ("#8.3");

   ASSERT_EQ (8, accuracy_parser.mantisse);
   ASSERT_EQ (3, accuracy_parser.decimals);


   Rule<Accuracy> rule;
   Accuracy accuracy;
   rule = "hallo" | AccuracyParser ();
   rule.parse ("#5.1", accuracy);

   ASSERT_EQ (5, accuracy.mantisse);
   ASSERT_EQ (1, accuracy.decimals);

   rule.parse ("#4", accuracy);
   ASSERT_EQ (4, accuracy.mantisse);
   ASSERT_EQ (0, accuracy.decimals);

}


TEST (ClassParserTest, testSkipParser)
{
   AccuracyParser  accuracy;

   accuracy.parse (SkipScanner<WhiteSpace>(" # 8\t.\n3"), fn::ignore);

   ASSERT_EQ (8, accuracy.mantisse);
   ASSERT_EQ (3, accuracy.decimals);
}


#endif


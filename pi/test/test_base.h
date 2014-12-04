#ifndef  _PARSER_TESTBASE_H
#define  _PARSER_TESTBASE_H

#include <iostream>
#include <pi/Parser.h>
#include <pi/Backref.h>
#include <util.h>
#include <type_traits>
#include <meta/is_chararray.h>

using namespace pi;
using namespace std;

typedef  pi::SkipScanner <WhiteSpace>  Skip;
typedef  pi::SkipScanner <WhiteSpace>  SkipScanner;


void  TestCase (const char* name, void (*test)())
{
   cout << "\n>> start test case: " << name << std::endl;
   test();
}

#define  TEST_CASE(test)   TestCase (#test, test)



template <class Scanner, class Parser>
void parse_assert (Parser &parser, const char *input, const char* result)
{
   cout << "\nParser: \n";
   parser.toString (cout, WRITEMODE::SURVEY);
   cout << "Input : ";
   mgo::pretty_print (cout, input); 
   cout << "\nResult: '";
   mgo::pretty_print (cout, result);
   cout << "\n-> ";
   cout.flush();

   Scanner  scanner (input);

   pi::mp::make_attribute <String>::type  attrib;
   if (parser.parse (scanner, attrib)  ||   attrib.Value() == result)
      cout << "TEST OK" << endl;
   else
      cout << endl <<  scanner.getErrorInfo();
}


template <class Parser>
void parse_assert (Parser &parser, const char *input, const char* result)
{
   parse_assert <pi::Scanner<>> (parser, input, result); 
}




template <class Result, class Parser, class Scanner>
ErrorInfo&  assertParserFailure (const Parser &parser, Scanner& scanner, Result& result)
{
   cout << "Start negative test\nParser: ";
   parser.toString (cout, WRITEMODE::EXPAND);
   cout << "\nInput : ";
   mgo::pretty_print (cout, scanner); 

   if (parser.parse (scanner, result))
      throw String ("ERROR! Parser should fail, but runs properly.");
   else
      cout << "\n-> TEST OK!\n" << endl;
   
   return  scanner.getErrorInfo();
}


template <class Result, class Parser>
ErrorInfo  assertParserFailure (const Parser &parser, const char* data, Result& result)
{
   Scanner<> scanner (data);
   assertParserFailure (parser, scanner, result);

   return scanner.getErrorInfo();
}



template <class Result, class Parser, class Scanner>
typename std::enable_if <!::mp::is_chararray<Result>::value, void>::type
assertParser (const Parser &parser, Scanner& scanner, const Result& result)
{
   cout << "Parser: \n";
   parser.toString (cout, WRITEMODE::EXPAND);
   cout << "\nInput : ";
   mgo::pretty_print (cout, scanner); 


   Result value;
   if (parser.parse (scanner, value))
   {
      if (value == result)  cout << "\n-> TEST OK!" << endl;
      else  throw String("ERROR: Parser result is wrong"); 
   }
   else  throw scanner.getErrorInfo();
}


template <class Parser, class Scanner>
void  assertParser (const Parser &parser, Scanner& scanner, const char* result)
{
   cout << "Parser: ";
   parser.toString (cout, WRITEMODE::EXPAND);
   cout << "\nInput : ";
   mgo::pretty_print (cout, scanner); 
   cout << "\nResult: ";
   mgo::pretty_print (cout, result); 

   String value;
   if (parser.parse (scanner, value))
   {
      if (value == result)  cout << "\n-> TEST OK!\n" << endl;
      else 
         throw String("ERROR: Parser result is wrong.\nExpected: ") + 
            result + "\nResult  : " + value;
   }
   else  throw scanner.getErrorInfo();
}


template <class Parser>
void  assertParser (const Parser &parser, const char* range, const char* result)
{
   Scanner<> scanner (range);
   assertParser (parser, scanner, result);
}

#endif

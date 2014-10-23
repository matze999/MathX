#include "../Parser.h"
#include "../Backref.h"



void  test_Backref()
{
   using namespace pi::backref;


   Rule0<String>  start =  "<" >> word >> ">",
                 end   =  "</" >> word >> ">";

   Rule0<String>  html  =  (_1 = start) >> *~char_("<") >> (_1 == end);

   assertParser (html, "<asd>hallo</asd>", "asdhalloasd");


   Rule0<String>  html2 = (_1 = word) >> ":" >> word >> ":" >> _1;
   parse_assert (html2, "asd:test:asd","asdtestasd");

   assertParserFailure (html2, "asd:test:zxc", String());
}




#include <fn.h>
//#include <stack.h>
#include "../Parser.h"
#include <string>
#include <iostream>

using namespace pi;



struct SkipParser: public Parser<>
{
    SkipParser(): Parser<> (ch::ws | ("//" >> (*char_ >> ch::eol)))  {}
};



class NousParser: public Grammer <SkipParser>
{
public:
   NousParser(): StartRule (nous)
   {
      word = (ch::alpha | ch::_) >> *ch::word;
      identifier = word;
      attribute  = word;

      key.add ("def",     1)
              ("theorem", 2)
              ("axiom",   3);

      entity =  key >> identifier >> "{" >> attribute >> "}";

      name_space = "namespace" >> identifier >> "{" >>  +entity  >> "}";

      nous = +name_space;
   }


   std::string&  getValue()
   {
      return  nous.getValue();
   }


private:
   Symbol   word, identifier, attribute;
   Rule <std::string>  nous, name_space, entity;
   Keyword <Token, int>  key;
};




void testNous()
{
   NousParser nous;

   std::cout << "size: " << sizeof (nous) << std::endl;

   parse ("namespace asd { def zxc { // comment \n attrib }}", nous);


}


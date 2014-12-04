#ifndef  __TEST_PARSER_GRAMMER_
#define  __TEST_PARSER_GRAMMER_


#include <gtest/gtest.h>
#include <pi/parser.h>


using namespace pi;



struct Person
{
   string name, address;
   int  age;

   Person () = default;

   Person (const string& name, const string& addr, int age):
      name (name), address (addr), age (age)  {}

   Person&  operator= (const string& text)
   {
      if (name.empty ())
         name = text;
      else
         address = text;

      return *this;
   }

   Person&  operator= (int val)
   {
      age = val;
      return *this;
   }

};

bool operator == (const Person& lhs, const Person& rhs)
{
   return lhs.name == rhs.name  &&
      lhs.address == rhs.address  &&
      lhs.age == rhs.age;
}


struct PersonParser: public Grammar < Person, WhiteSpace >
{
   PersonParser (): start_rule (person_p)
   {
      person_p = word >> "," >> -(word >> ",") >> int_;
   }

   Rule<Person>  person_p;
};


TEST (GrammerTest, testSimpleStruct)
{
   PersonParser person_p;
   Person  person;

   person_p.parse ("Robi, \tLocal, \n21", person);
   ASSERT_EQ (person, Person("Robi", "Local", 21));
}



#endif


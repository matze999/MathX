#include <util.h>

struct Person: Grammer <Person, WhiteSpace>
{
   Person(): StartRule (start)
   {
      is_name_assign = true;
      start =  word(name) >> "," 
           >> -(word(address) >> ",")
           >>  int_(age);
   }

   Rule<Person> start;


   std::string name;
   std::string address;
   int    age;

   void clear()  { is_name_assign = true; }

   Person&  operator= (const String& str)
   {
      if (is_name_assign)  name = str;
      else                 address = str;
      is_name_assign = !is_name_assign;
      return *this;
   }

   Person&  operator= (int val)
   {
      age = val;
      return *this;
   }

   Person&  operator<< (const String& str)
   {
      if (is_name_assign)  name = str;
      else                 address = str;
      is_name_assign = !is_name_assign;
      return *this;
   }

   Person&  operator<< (int val)
   {
      age = val;
      return *this;
   }

private: 
   bool  is_name_assign;
};



std::ostream&  operator<< (std::ostream& out, const Person& person)
{
   return  out << "<name: " << person.name << "; address: " << person.address << "; age: " << person.age << ">";
}



void testPersonParser()
{
  const char*  text = "andre,home,51\nRobert,Kroatien,1234";

   Rule <Person>  person_p;
   Rule <std::vector<Person>>  start_p;


   person_p =  word >> "," >> -(word >> ",") >> int_;
   start_p  = +(person_p >> ignore (ch::eol));

   cout << start_p << endl;

   std::vector<Person> result;
   start_p.parse (text, result);

   assert (result.size () == 2)
   //person_grammer.parse (scanner); 

}



void testPersonGrammer()
{
   pi::Range  range ("andre,home,51\nRobert,Kroatien,1234");

   Person  person_;

   //std::vector<Person>  persons;
   std::vector<Person> result;

   person_.name = "TEST";
   result.push_back (person_);

   Rule <std::vector<Person>>  start_(result);

   start_  = +person_;

   cout << start_ << endl;
   start_.parse (range);

   //size_t  len = start_p.getValue().size();
   //person_grammer.parse (scanner); 

}


struct Person2
{
   std::string name;
   std::string address;
   int    age;
};


struct PersonBuilder: public Person2, public ParserBuilder<PersonBuilder, WhiteSpace>
{
   template <class Scanner>
   bool  read (Scanner& scanner)
   {
      auto parser = word(name) >> "," 
                  >> -(word(address) >> ",") 
                  >> int_(age);

      return  parser.parse (scanner, pi::fn::no_action());
   }
};


void testPersonBuilder()
{
   pi::Range  range ("andre,home,51\nRobert,Kroatien,1234");

   PersonBuilder  builder;
   Rule<std::vector<Person2>>  rule = +builder;
   rule.toString (cout);

   //TRC_LEVEL = 4;
   rule.parse (range);

   
   const std::vector<Person2>&  result = rule.Value();

   assert (result.size() == 2);
}




#include "JsonParser.h"
#include "JsonWriter.h"


void testJson()
{
   const char* data = 
"{ \n\
   hallo : [2, 3, 4],\n\
   emptyclass : {}, \n\
   next: \"kein eintrag\",\n\
   end: 'empty empty',\n\
   'object 1': {\n\
      nullnode: null, \n\
      emptylist : [], \n\
      \"empty class\" : {}, \n\
      bool: true }\n\
}";

   pi::Range  range = data;
//"{ hallo : [2, 3, 4],\
//next: \"kein eintrag\",\
//end: 'empty empty',\
//'object 1': {\
//bool: true,\
//empty list: [],\
//empty class : {}, \
//null node: null }\
//}";
   // empty: null, vector: [], bool: true 

   json::Allocator allocator;


   json::Parser parser(allocator);
   //parser.toString (cout, WRITEMODE::START);
   //cout << endl;


   if (!parser.parse (range))
      cout << scanner.getErrorInfo() << endl;

   json::ObjectRef  result = parser;
      
   //json::BaseWriter(result).toString (cout);
   //cout << endl;
   //cout << result << endl;


   json::Parser2  parser2 (allocator);
   pi::Range  range2 = data;
   if (!parser2.parse (range2))
      cout << scanner.getErrorInfo() << endl;

   const json::ObjectAttribute2&  result2 = parser2.Value();
      
   //json::BaseWriter(result).toString (cout);
   //cout << endl;
}



void testGrammer ()
{
   testJson();
   testPersonParser();
   testPersonGrammer();
   testPersonBuilder();
}


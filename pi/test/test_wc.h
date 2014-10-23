#include "../primitive/Charclass.h"


struct  Counter: public pi::vector_attribute_tag
{
   Counter ()   
   { 
      clear(); 
   }

   void clear()
   {
      value = 0;
   }

   template <class T>
   void push_back (const T& arg)
   {
      ++value;
   }

   unsigned value;
};



void testWC ()
{
   using namespace pi;

   std::string text = "Ich hoffe, dass wird jetzt gehen! Nicht wahr.\n"
      "Und was glaubst Du?\n"
      "Das ist eine dritte Zeile";

   Scanner<>  scanner(text.data(), text.size());

   Counter  counter;

   // char count
   cout << "count character: " << endl;
   counter.clear();
   (*char_).parse (scanner, counter);
   assert0 (counter.value == text.size());

   // word count 1
   cout << "count words 1: ";
   auto end_p  = *(char_ / pi::ch::space);


   auto  word_p = pi::ignore (*pi::ch::space) 
               >> char_ >> pi::ignore(end_p);

   auto  word_count_p = *word_p;
   cout << word_count_p << endl;

   counter.clear();
   scanner.reassign (text.data(), text.size());

   word_count_p.parse (scanner, counter);
   assert (counter.value == 17);


   // words count 2
   auto  space2_  = *pi::ignore (ch::space);

   Rule0<String>  word2_ =  *(char_ / ch::space);

   counter.clear();
   Rule<Counter>  word_count2_ (counter);
   word_count2_= *(space2_ >> word2_);

   cout << "count words 2: ";
   word_count2_.toString (cout, WRITEMODE::COMPACT);

   scanner.reassign (text.data(), text.size());
   word_count2_.parse (scanner, counter);
   assert (counter.value == 17);

   // line count
   //Rule<>  line_p =  pi::ch::eol  
   //               | +(char_ / pi::ch::eol) >> -pi::ch::eol;
   //auto line_count_p  = *line_p [fn::val (' ')];

   //char_count.clear();
   //line_count_p.parse (pi::Range(text), char_count);
   //assert0 (char_count.getValue() == 3);


   Rule0<char>  line2_p =  pi::ch::eol  
                       | +(char_ / pi::ch::eol) >> -pi::ch::eol;

   auto line_count2_p  = *line2_p;

   cout << "line counter: " << endl;
   line2_p.toString (cout);

   counter.clear();
   scanner.reassign (text.data(), text.size());
   line_count2_p.parse (scanner, counter);
   assert0 (counter.value == 3);
}
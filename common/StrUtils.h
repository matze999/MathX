#ifndef _STR_UTIL_H_
#define _STR_UTIL_H_

#include <ostream>
#include <sstream>
#include "util.h"
#include "meta/typetraits.h"

namespace mgo {

const char*  pretty_char (char ch)
{
   static char  str[2] = {0};

   if (ch > 31) 
   {
      str[0] = ch;
      return str;
   }
   if (ch == '\n')  return  "\\n";
   if (ch == '\t')  return  "\\t";
   return "\?";
}


void pretty_print (std::ostream& out, const char* str)
{
   while (*str)
   {
      out << pretty_char (*str++);
   }
}


void pretty_print (std::ostream& out, const std::string& string)
{
   with (string)  
      out << pretty_char (string[pos_]);
}


template <class Range>
void pretty_print (std::ostream& out, Range& range)
{
   typename Range::Iterator pos = range.begin();

   while (pos != range.end())
   {
      out << pretty_char(*pos);
      ++pos;
   }
}

std::string  pretty_string (const std::string& str)
{
   std::stringstream out;
   pretty_print (out, str);
   return  out.str ();
}


std::string  toString (char value)
{
   return  std::string () + value;
}



template <class T>
typename mp::enable_if <mp::is_arithmetic <T>::value, std::string>::type  
toString (const T& value)
{
   std::stringstream out;
   out << value;
   return out.str ();
}



void erase_all (std::string& data, const char*  str)
{
   size_t pos = 0;
   while (true)
   {
      pos = data.find (str, pos);
      if (pos == std::string::npos)  return;
      data.erase (pos, strlen (str));
   }
}


void replace_all (std::string& data, const char* old, const char* str)
{
   size_t pos = 0;
   while (true)
   {
      pos = data.find (old, pos);
      if (pos == std::string::npos)  return;
      data.replace (pos, strlen (old), str);
   }
}



template <class T>
std::string getTypeIdHelper () 
{
   std::string  name = typeid (T).name();

   erase_all (name, "class ");
   erase_all (name, "struct ");
   erase_all (name, "std::");

   replace_all (name, "basic_string<char,char_traits<char>,allocator<char> >", "string");
   return name;
}


#define  getTypeId(T)  mgo::getTypeIdHelper<T>()
#define  TypeName(T)  mgo::getTypeIdHelper<T>()

} // namespace mgo
#endif
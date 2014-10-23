#ifndef  __JSONPARSER_H__
#define  __JSONPARSER_H__


#include "JsonItem.h"

#include "../parser.h"



namespace json {
using namespace pi;



typedef  Attribute <vector_attribute_tag, List>  ListAttribute;



class ObjectAttribute: public Attribute <vector_attribute_tag, Object>
{
public:
   ObjectAttribute (ObjectAttribute& other):
      Attribute<Object&> (other.Value()),  is_new_entry (true)  {}

   ObjectAttribute (Object& obj): 
     Attribute<Object&> (obj), is_new_entry (true)  {}


   bool  operator() (const std::string&  str)
   {
      if (is_new_entry)
      {
         is_new_entry = false;
         name = str;
      }
      else
      {
         is_new_entry = true;
         if (str == "null")
            Value()[name].setNull();
         else
            Value()[name] = str;

         TRC_DEBUG(json) << "add to object" << endl;
      }
      return  true;
   }

   template <class T>
   bool  operator() (const T& value)
   {
      if (is_new_entry)  return false;

      Value()[name] = value;
      return  is_new_entry = true;
      TRC_DEBUG(json) << "add to object" << endl;
   }

private:
   std::string  name;
   bool is_new_entry;
};


struct Parser;

} //namespace json


namespace pi { namespace mp {
   template <>
   struct ::pi::mp::make_attribute <json::Parser&>: public
      promote <json::ObjectAttribute>  {};
}}


namespace json {


struct Parser: public Object, public Grammer <Parser, WhiteSpace>
{
   Parser (const Parser& other):
      Object (other.getAllocator()),
      StartRule (start_),
      list_ (other.getAllocator())  
   {
      makeParser();
   }

   Parser (Allocator &allocator): 
      Object (allocator),
      StartRule (start_),
      list_ (allocator)
   {
      makeParser();
   }

   void makeParser()
   {
      auto   quote = ignore (char_("'\""));
      string_ = word
              | no_skip (quote >> char_* quote); 

      auto item_ = int_
                 | bool_
                 | string_
                 | list_
                 | *this;

      list_ = "[" >> item_ % "," >> "]";
      
      auto object_ = "{" >> (string_ >> ":" >> item_) % "," >> "}";
      start_ = object_(*this);
   }

private:
   Rule <std::string>      string_;
   Rule <ListAttribute>    list_;
   Rule <>  start_;
};




class ObjectAttribute2: public Attribute<Object>
{
public:
   ObjectAttribute2 (ObjectAttribute2& other):
      Attribute<Object> (other.getAllocator()),  is_new_entry (true)  {}

   ObjectAttribute2 (Allocator& alloc): 
     Attribute<Object> (alloc), is_new_entry (true)  {}


   bool  operator() (const std::string&  str)
   {
      if (is_new_entry)
      {
         is_new_entry = false;
         name = str;
      }
      else
      {
         is_new_entry = true;
         if (str == "null")
            Value()[name].setNull();
         else
            Value()[name] = str;

         TRC_DEBUG(json) << "add to object" << endl;
      }
      return  true;
   }

   template <class T>
   bool  operator() (const T& value)
   {
      if (is_new_entry)  return false;

      Value()[name] = value;
      return  is_new_entry = true;
      TRC_DEBUG(json) << "add to object" << endl;
   }

private:
   std::string  name;
   bool is_new_entry;
};



struct Parser2: public Grammer2<ObjectAttribute2, WhiteSpace>
{
   Parser2 (Allocator &allocator):
      StartRule (object_),
      object_ (allocator),
      list_ (allocator)
   {
      makeParser();
   }

   void makeParser()
   {
      auto   quote = ignore (char_("'\""));
      string_ = word
              | no_skip (quote >> char_* quote); 

      auto item_ = int_
                 | bool_
                 | string_
                 | list_
                 | object_;

      list_ = "[" >> item_ % "," >> "]";
      
      object_ = "{" >> (string_ >> ":" >> item_) % "," >> "}";
   }

private:
   Rule <std::string>      string_;
   Rule <ListAttribute>    list_;
   Rule <ObjectAttribute2>  object_;
};


} // namespace json
#endif


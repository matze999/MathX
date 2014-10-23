#ifndef  __JSONWRITER_H__
#define  __JSONWRITER_H__


#include "JsonItem.h"

namespace json {



class BaseWriter
{
protected:
   Item start;
public:
   BaseWriter (Item& item): start(item)  {}


   void toString (std::ostream& out)
   {
      toString (out, start);
   }

   void toString (std::ostream& out, Item& item)
   {
      switch (item.Type())
      {
      case ObjectType::Null:
         out << "null";
         break;

      case ObjectType::Integer:
         out << (int) item;
         break;

      case ObjectType::Bool:
         //out << ((bool) item? "true": "false");
         break;

      case ObjectType::String:
         writeString (out, (const std::string&) item);
         break;

      case ObjectType::List:
         writeList (out, (List&) item);
         break;

      case ObjectType::Object:
         writeObject (out, (Object&) item);
         break;
      }
   }

   void  writeString (std::ostream& out, const std::string& val)
   {
      out << '"' << val << '"';
   }

   void  writeObject (std::ostream& out, ObjectRef object)
   {
      out << '{';
      if (!object.empty()) 
      {
         ObjectRef::iterator  iter = object.begin(),
                              end  = object.end();

         uint  pos = 0, last = object.size() - 1;
         while (iter != end)
         {
            out << iter->first << ": ";
            Item  item (&iter->second, object.getAllocator());
            toString (out, item);
            if (pos != last) out << ", ";

            ++pos; ++iter;
         }
      }
      out << '}';
   }

   void  writeList (std::ostream& out, ListRef list)
   {
      out << '[';
      with (Item item, list)
      {
         toString (out, item);
         if (pos_ != last_) out << ", ";
      }
      out << ']';
   }
};



class Tabulator
{
public:
   Tabulator (): indent(0)  {}
   Tabulator (size_t val): indent(val)  {}

   Tabulator&  operator++ ()     { ++indent; return *this; }
   Tabulator&  operator-- ()     { --indent; return *this; }

   friend
   std::ostream&  operator<< (std::ostream& out, Tabulator& tab)
   {
      if (tab.indent)  out << "   ";
      for (size_t pos = 1; pos < tab.indent; ++pos)
            out << "   ";
      return  out;
   }

protected:
   size_t indent;
};



class PrettyTab: public Tabulator
{
   friend
   std::ostream&  operator<< (std::ostream& out, PrettyTab& tab)
   {
      if (tab.indent)  out << "   ";
      for (size_t pos = 1; pos < tab.indent; ++pos)
            out << ".  ";
      return  out;
   }
};


class PrettyWriter: public BaseWriter
{
public:
   PrettyWriter (Item& item): BaseWriter (item)  {}

   void toString (std::ostream& out)
   {
      toString (out, start);
   }


   void toString (std::ostream& out, Item& item)
   {
      switch (item.Type())
      {
      case ObjectType::Integer:
         out << (int) item;
         break;

      case ObjectType::String:
         writeString (out, (const std::string&) item);
         break;

      case ObjectType::List:
         writeList (out, (List&) item);
         break;

      case ObjectType::Object:
         writeObject (out, (Object&) item);
         break;

      case ObjectType::Bool:
         out << ((bool) item? "true": "false");
         break;

      case ObjectType::Null:
         out << "null";
         break;
      }
   }


   void  writeObject (std::ostream& out, ObjectRef object)
   {

      if (object.empty()) 
         out << "{}";
      else
      {
         out << "{\n";
         ++tab;
         ObjectRef::iterator  iter = object.begin(),
                              end  = object.end();

         //uint  pos = 0, last = object.size() - 1;

         out << tab << iter->first << ": ";
         Item  item (&iter->second, object.getAllocator());
         toString (out, item);

         while (++iter != end)
         {
            out << ",\n"<< tab << iter->first << ": ";

            Item  item (&iter->second, object.getAllocator());
            toString (out, item);
         }
         --tab;
         out << '\n' << tab  << '}';
      }
   }

   void  writeList (std::ostream& out, ListRef list)
   {
      if (list.empty())  cout << "[]";
      else
      {
         out << "[\n";
         ++tab;

         with (Item item, list)
         {
            out << tab;
            toString (out, item);
            if (pos_ != last_)  out << ",\n";
         }

         --tab;
         out << '\n' << tab  << ']';
      }
   }

private:
   PrettyTab  tab;
};



std::ostream&  operator<< (std::ostream& out, Item& item)
{
   PrettyWriter(item).toString (out);
   return out;
}


} // namespace json
#endif


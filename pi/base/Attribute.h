#ifndef  _PARSERATTRIBUTE__
#define  _PARSERATTRIBUTE__


//#pragma warning (disable: C4521)  // multiple copy constructors specified

#include "../base/Parser.h"
//#include "primitives.h"
#include <meta.h>
#include <meta/is_vector.h>
#include <Wrapper.h>

 

namespace pi { 
 



template <class Cat>
struct  attribute
{
   typedef  Cat  category;
};

struct  assign_attribute_tag: public attribute <assign_attribute_tag> {};
struct  vector_attribute_tag: public attribute <vector_attribute_tag> {};
struct  string_attribute_tag: public attribute <string_attribute_tag> {};
struct  stream_attribute_tag: public attribute <stream_attribute_tag> {};
struct  inserter_attribute_tag: public attribute <inserter_attribute_tag> {};
struct  functional_attribute_tag: public attribute <functional_attribute_tag> {};


template <class T>
struct  is_attribute: public  mp::is_template_of <attribute, T>  {}; 




template <class T, bool = is_attribute<T>::value>
struct  attribute_traits
{
   typedef  typename T::category  category;
};

template <class T>
struct  attribute_traits <T, false>: 
   public assign_attribute_tag  {};




template <class T>
typename  attribute_traits<T>::category  attribute_category (const T&)
{
   return  typename attribute_traits<T>::category();
}



using mgo::Assoc;



template <class category, class T, Assoc assoc = Assoc::Composite>
class Attribute: public mgo::Wrapper<T, assoc>, public category
{
   typedef  mgo::Wrapper<T, assoc>  Base;
   static_assert (is_attribute<category>::value, "expect an attribute tag");
public:
   Attribute ()  {}

   //Attribute (Attribute&  other): Base(other.Value())  {}
   Attribute (const Attribute&  other): Base(other)  {}

   Attribute (T&  other): Base(other)  {}
   Attribute (const T&  other): Base(other)  {}


   template <class U>
   void  push_back (const U& value)
   {
      Value().push_back (value);
   }

   template <class U>
   void  operator<< (const U& value)
   {
      Value() << value;
   }

   template <class U>
   void  operator+= (const U& value)
   {
      Value() += value;
   }

   template <class U>
   void  operator() (const U& value)
   {
      Value() (value);
   }

   Attribute&  operator= (const T& other)
   {
      Value() = other;
      return *this;
   }

   Attribute&  operator= (const Attribute& other)
   {
      Value() = other.Value();
      return *this;
   }
};



namespace mp {
using namespace ::mp;

template <class T, bool = is_attribute<T>::value>
struct  attribute_category
{
   typedef typename
   if_ <::mp::is_vector<T>::value,            vector_attribute_tag,
   if_ <is_base_of <std::string, T>::value,   string_attribute_tag,
   if_ <is_base_of <std::ostream, T>::value,  stream_attribute_tag,
                                              assign_attribute_tag
   >>>::type type;
};

template <class T>
struct  attribute_category <T, true>
{
   typedef  typename T::category  type;
};


template <class Attrib>
struct  attribute_assign
{
   template <class Value>
   static void  execute  (Attrib&  attribute, const Value& value, assign_attribute_tag)
   {
      attribute = value;
   }
 
   template <class Value>
   static void  execute  (Attrib&  attribute, const Value& value, vector_attribute_tag)
   {
      attribute.push_back (value);
   }

   template <class Value>
   static void  execute  (Attrib&  attribute, const Value& value, string_attribute_tag)
   {
      attribute += value;
   }

   template <class Value>
   static void  execute  (Attrib&  attribute, const Value& value, stream_attribute_tag)
   {
      attribute << value;
   }

   template <class Value>
   static bool  execute  (Attrib&  attribute, const Value& value, inserter_attribute_tag)
   {
      return  attribute.insert (value);
   }

   template <class Value>
   static void  execute  (Attrib&  attribute, const Value& value, functional_attribute_tag)
   {
      attribute (value);
   }
};


template <class Attrib, class Value>
void  assign  (Attrib&  attribute, const Value& value)
{
   typedef typename attribute_category<Attrib>::type  category;
   attribute_assign <Attrib>::execute (attribute, value, category());
}


template <class Attrib>
void  assign  (Attrib&  attribute, const Attrib& value)
{
   attribute = value;
}


void  assign  (std::string&  attribute, const std::string& value)
{
   attribute += value;
}





template <class T, Assoc  assoc = Assoc::Composite>
struct make_attribute
{
   // typedef  mgo::Wrapper <T, assoc>  type;
   typedef typename
   if_ <is_attribute<T>::value,               T,
   if_ <::mp::is_vector<T>::value,            Attribute <vector_attribute_tag, T, assoc>,
   if_ <is_base_of <std::string, T>::value,   Attribute <string_attribute_tag, T, assoc>,
   if_ <is_base_of <std::ostream, T>::value,  Attribute <stream_attribute_tag, T, assoc>,
                                              T
   >>>>::type type;
};



} // namespace mp
} // namespace pi
#endif

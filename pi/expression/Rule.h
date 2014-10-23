#ifndef  _PARSERULE__
#define  _PARSERULE__

#include <VectorUtils.h>
#include <map>
#include <Reference.h>
#include "../base/Parser.h"
#include "../base/Attribute.h"
#include <meta/typetraits.h>

#include <memory>

namespace pi { 
namespace _ {



template <class Scanner, class Attribute>
struct base_p
{
public:
   typedef Attribute  value_type;

   base_p()                      { id = createId(); }

   virtual  ~base_p() {}

   virtual  bool  parse (Scanner &scanner, Attribute& attrib) const = 0;


   virtual  void toString (std::ostream &out, WRITEMODE level) const = 0;

   int id;
};



template <class Scanner, class Attribute, class Parser>
struct proxy_p: public base_p <Scanner, Attribute>
{
   proxy_p (const Parser &p): child(p)  {}


   virtual  bool parse (Scanner &scanner, Attribute&  attribute) const 
   {
      TRC_INDENT_SECTION (4, parser);
      return  child.parse (scanner, attribute);
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      child.toString (out, level);
   }

protected:
   typename ref_to<Parser>::type    child;
};



static  int  createId()
{
   static int id = 0;
   return ++id;
}


typedef  std::map <std::string, std::string>  StringMap;


StringMap&  RuleMap()
{
   static StringMap rule_map;
   return rule_map;
}




template <class Attribute, class Scanner>
struct rule0_p
{
   enum { rank =  PID::RULE };

   rule0_p (): child (nullptr)  
   {
      name = "Rule";
      name += createId ();
   }


   ~rule0_p ()
   {
      delete child;
   }


   unique_ptr<Attribute> parse (Scanner& scanner) const
   {
      auto  attribute = std::make_unique <Attribute> (prototype);
      // unique_ptr<Attribute>  attribute = new Attribute;

      if (!parse (scanner, *attribute))
         attribute.release ();

      return attribute;
   }

   bool  parse (Scanner &scanner, Attribute&  attribute) const 
   { 
      if (child->parse (scanner, attribute))  return true;
      else 
         return  RuleFailure (scanner, name);
   }
   

   template <class Collector>
   bool  parse (Scanner &scanner, Collector&  collect) const 
   { 
      Attribute  attribute;
      if (!child->parse (scanner, attribute))  
          return  RuleFailure (scanner, name);

      mp::assign (collect, attribute);
      return true;
   }


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      if (level == WRITEMODE::SURVEY)
      {
         RuleMap().clear();
         insertRuleString();

         // write head
         out << name << " := " << RuleMap()[name] << std::endl;

         for (auto& item: RuleMap())
         {
            if (item.first != name)
               out << item.first << " := " << item.second << std::endl;
         }
      }
      else if (level == WRITEMODE::EXPAND)
      {
         insertRuleString();
         out << RuleMap()[name];
      }
      else if (level == WRITEMODE::COMPACT)
      {
         out << name;
         insertRuleString();
      }
      else  
         out << name;
   }


   template <class P>
   void  assign (const BaseParser<P> &other)
   {
      child = new  proxy_p <Scanner, Attribute, P> (other);
   }

	void  Name (const char* name)
	{
		this->name = name;
	}

protected:
   rule0_p (const rule0_p&) = delete;


   void  insertRuleString() const
   {
      if (RuleMap().count (name) == 0)
      {
         RuleMap() [name] = "";
         std::stringstream  out;
         child->toString (out, WRITEMODE::COMPACT);

         RuleMap()[name] = out.str();
      }
   }


   //mgo::Reference <base_p <Scanner, Attribute>*>  child;
   std::string  name;
   base_p <Scanner, Attribute>*  child;
};


template 
<
   class Attribute, 
   class Scanner
>
struct rule_p: public rule0_p <Attribute, Scanner>
{
   template <class ...Args>
   rule_p (Args&&... args): prototype (args...)  {}


   std::unique_ptr<Attribute> parse (Scanner& scanner) const
   {
      auto  attribute = std::make_unique<Attribute> (prototype);

      if (!parse (scanner, *attribute))
         attribute.release ();

      return attribute;
   }
    
   bool  parse (Scanner &scanner, Attribute&  attribute) const 
   { 
      if (child->parse (scanner, attribute))  return true;
      else return  RuleFailure (scanner, name);
   }
   
   template <class Collector>
   bool  parse (Scanner &scanner, Collector&  collect) const 
   { 
      Attribute  attribute = prototype;
      if (!child->parse (scanner, attribute))  
         return  RuleFailure (scanner, name);

      mp::assign (collect, attribute);
      return true;
   }

protected:
   rule_p (const rule_p&) = delete;

   Attribute  prototype;
};



template <class T, class Scanner>
struct ref_to <rule_p <T, Scanner>>
{
   typedef  const pi::_::rule_p <T, Scanner>&  type;
};

template <class T, class Scanner>
struct ref_to <rule0_p <T, Scanner>>
{
   typedef  const pi::_::rule0_p <T, Scanner>&  type;
};


} // namespace _





//*** Rule

template 
<
   class Attribute,
   class Scanner   = pi::Scanner<>
>
class Rule0: public BaseParser <_::rule0_p <Attribute, Scanner>>
{
public:
   Rule0 () = default;

   template <class P>
   Rule0 (const BaseParser<P> &other)
   {
      assign (other);
   }


   template <class P>
   Rule0&  operator= (const BaseParser<P> &other)
   {
      assign (other);
      return *this;
   }


private:
   Rule0 (const Rule0& other) = delete;
   Rule0&  operator= (const Rule0&) = delete;
};



template 
<
   class Attribute,
   class Scanner   = pi::Scanner<>
>
class Rule: public BaseParser <_::rule_p <Attribute, Scanner>>
{
public:

   template <class ...Args>
   Rule (Args&&...  args): Super (args...)   {}


   template <class P>
   Rule&  operator= (const BaseParser<P> &other)
   {
      assign (other);
      return *this;
   }

private:
   Rule (const Rule& other) = delete;
   Rule&  operator= (const Rule&) = delete;
};



} // namespace pi
#endif

#ifndef  _GRAMMER2__
#define  _GRAMMER2__

#include  "rule.h"
#include  "../scanner/Scanner.h"


namespace pi { 


namespace _ {


template <class T, class SkipParser>
class grammer2_p
{
protected:
   typedef  pi::SkipScanner <SkipParser>   SkipScanner;
   typedef  pi::Rule <T, SkipScanner>    Rule0;
public:
   typedef  T value_type;

   static const int  rank = PID::EXPRESSION;

   grammer2_p (Rule0 &parser): start(parser)  {}


   template <class Scanner>
   bool  parse (Scanner &scanner) const 
   {
      SkipScanner skip_scanner = scanner;
      return  start->parse (skip_scanner);
   }

   template <class Scanner, class Collector>
   bool parse (Scanner &scanner, Collector &collect) const
   {
      SkipScanner skip_scanner = scanner;
      return  start->parse (skip_scanner, collect);
   }

   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::COMPACT) const
   {
      out << "Gramer <" << getTypeId(T) '>';
      //start->toString (out, level);
   }

   const T&  Value() const
   {
      return  start->Value();
   }

private:
   mgo::Reference<Rule0>  start;
};


} // namespace _



template <class Base, class SkipParser = void>
class  Grammer: public BaseParser <_::grammer2_p <Base, SkipParser>>
{
   typedef  pi::SkipScanner <SkipParser>   SkipScanner;
   //typedef  pi::Rule <fn::no_action, SkipScanner>    Rule0;
public:
   typedef  Grammer  StartRule;


   Grammer (typename StartRule::Rule0& start)
      : BaseParser <_::grammer2_p <Base, SkipParser>> (start)  {}


//*** redefine Rule 

   template <class Attribute>
   using Rule = pi::Rule <Attribute, SkipScanner>;
};


} // namespace pi
#endif


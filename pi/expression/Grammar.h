#ifndef  _PARSER_GRAMMER__
#define  _PARSER_GRAMMER__

#include  "../scanner/Scanner.h"
#include  "Rule.h"
#include  "ClassParser.h"

namespace pi { 


namespace _ {


template <class T, class SkipParser>
class grammar_p
{
protected:
   using  SkipScanner = pi::SkipScanner <SkipParser>;
   using  GrammarRule = pi::Rule <T, SkipScanner>;
public:
   typedef  T value_type;

   static const int  rank = PID::EXPRESSION;

   grammar_p (GrammarRule &parser): start(parser)  {}


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
      out << "Grammar <" << getTypeId(T) '>';
      //start->toString (out, level);
   }

   const T&  Value() const
   {
      return  start->Value();
   }

private:
   mgo::Reference<GrammarRule>  start;
};


} // namespace _



template <class T, class SkipParser = void>
class  Grammar: public BaseParser <_::grammar_p <T, SkipParser>>
{
public:
   using  StartRule =  Grammar;
   using  start_rule =  Grammar;

   using  scanner_type = pi::SkipScanner <SkipParser>;
   using  GrammarRule = pi::Rule <T, scanner_type>;



   Grammar (GrammarRule& start): Super (start)  {}


//*** redefine Rule

   template <class Attribute>
   using Rule0 = pi::Rule0 <Attribute, scanner_type>;

   template <class Attribute>
   using Rule = pi::Rule <Attribute, scanner_type>;

};


} // namespace pi
#endif


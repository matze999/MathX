#ifndef  _SKIPSCANNER_H_
#define  _SKIPSCANNER_H_


#include "../operator/Repeat.h"
#include "../scanner/Scanner.h"



namespace pi {


class skip_scanner_tag {};

template <class Parser, class Scanner = pi::Scanner>
class  SkipScanner: private skip_scanner_tag
{
   typedef  _::repeat_p <Parser, 0, -1>   SkipParser;
public:
   typedef  typename Scanner::Iterator  Iterator;
   typedef  Scanner            BaseScanner;

   SkipScanner (const Parser &parser = Parser()): 
      skip_parser (parser)
   {
   }

   SkipScanner (Scanner &scanner): skip_parser (Parser())
   {
      operator= (scanner);
   }

   SkipScanner&  operator= (Scanner &scan)
   {
      if ((Scanner*) this != &scan)  scanner = &scan;
      skip();
      return *this;
   }


   void  pop_front()
   {
      scanner->pop_front();
      skip();
   }

   char  front() const
   {
      return scanner->front();
   }


   Iterator  begin()
   {
      return scanner->begin();
   }

   void  begin (Iterator&  p)
   {
      scanner->begin (p);
   }

   bool  empty() const
   {
      return scanner->empty();
   }

   void  skip() const
   {
      TRC_DEBUG (scanner) << "call skip parser" << mgo::endl;
      int level = TRC_LEVEL;
      TRC_LEVEL = min (level, 3);

      skip_parser.parse (*scanner, fn::no_action());
      TRC_LEVEL = level;
   }

   operator bool() const
   {
      return !empty();
   }

   void  invalidate (const char * msg = "")
   {
      scanner->invalidate (msg);
   }

   ErrorInfo&  getErrorInfo()
   {
      return  scanner->getErrorInfo();
   }

   operator BaseScanner& ()
   {
      return *scanner;
   }

   template <class Parser>
   void  addMessage(PARSING_ERROR, const Parser &parser)
   {
      scanner->addMessage(PARSING_ERROR, parser);
   }

private:
   SkipParser    skip_parser;
   mutable  Scanner     *scanner;
};




} // namespace pi
#endif 


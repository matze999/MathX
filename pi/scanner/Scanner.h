#ifndef  _SCANNER_H_
#define  _SCANNER_H_


#include <stack.h>
#include <sstream>
#include "../operator/Repeat.h"
#include "../base/Base.h"
#include <ctype.h>
#include <Wrapper.h>
#include "BaseScanner.h"
#include "StrUtils.h"


namespace pi {



//namespace _ {


struct UnskipFun
{
   UnskipFun () {}

   template <class T>
   UnskipFun (T) {}

   template <class Scanner>
   void  operator() (Scanner& scanner) const  {}
};

template <class Parser>
class  SkipFun
{
   typedef  _::kleenestar_p<Parser>   SkipParser;
   SkipParser    skip_parser;
public:

   SkipFun (Parser&  parser = Parser()): skip_parser(parser)  {}

   template <class T>
   SkipFun (T& other): skip_parser (Parser())  {}

   template <class Scanner>
   void  operator() (Scanner&  scanner)  const
   {
      TRC_DEBUG (scanner) << "call skip parser" << mgo::endl;
      TRC_SECTION (TL_INFO)
      {
         skip_parser.parse (scanner.getNoSkipScanner(), fn::no_action());
      }
   }
};


struct NoTransformer
{
   char operator() (char ch) const  { return ch; }
};

struct  UpperCaseTransformer
{
   char operator() (char value) const
   { 
      return  toupper(value); 
   }
};




template 
<
   class Range = mgo::Range <const char*>,
   class Skipper = UnskipFun, 
   class Transformer = NoTransformer
>
class Scanner
{
   typedef   ScannerData<Range>  Base;
public:

   template <class Parser>
   struct    SkipScanner
   {  
      typedef  Scanner <Range, SkipFun<Parser>, Transformer>  type;
   };

   typedef  Scanner <Range, UnskipFun, Transformer>    NoSkipScanner;

   typedef  Scanner <Range, Skipper, UpperCaseTransformer>  UpperCaseScanner;
   typedef  Scanner <Range, Skipper, NoTransformer>         NormalCaseScanner;



   typedef  typename Base::iterator  Iterator;

   template <class R, class S, class T>
   friend  class Scanner;


// constructors

   Scanner (const char*  data, size_t leng = 0): 
      base ( new Base (data, data + (leng? leng: strlen(data)) ))
   {
      skipfun (*this);
   }

   template <class Parser>
   Scanner (const char*  data, Parser& parser):
      base ( new Base (data, data + (leng? leng: strlen(data)) ))
      skip (parser)
   {
      skipfun (*this);
   }


   Scanner (Range& range): base (new Base(range)) 
   {
      skipfun (*this);
   }

   template <class Parser>
   Scanner (Range& range,  Parser& parser): base (new Base(range)), skip (parser)
   {
      skipfun (*this);
   }


   template <class Skip2, class Transform2>
   Scanner (const Scanner<Range, Skip2, Transform2>& other):
      base (other.base), skipfun (other.skipfun)
   {
      skipfun (*this);
   }

   template <class Skip2, class Transform2, class Parser>
   Scanner (const Scanner<Range, Skip2, Transform2>& other,  Parser& parser): 
      base (other.base), skipfun (parser)
   {
      skipfun (*this);
   }


   void  reassign (const char*  data, size_t leng = 0)
   {
      base->start_pos = data;
      base->pos = base->start_pos;
      base->end_pos = data + (leng? leng: strlen(data));
      skipfun (*this);
   }


   void  skip ()
   {
      skipfun (*this);
   }


   char  front() const
   {
      return  transform (*base->pos);
   }

   void  pop_front()
   {
      ++base->pos;
      skipfun (*this);
   }

   bool empty () const
   {
      return  base->pos == base->end_pos;
   }

   Iterator  begin() const
   {
      return base->pos;
   }

   Iterator  end () const
   {
      return base->end_pos;
   }

   void  begin (Iterator&  pos)
   {
      base->pos = pos;
   }


// other scanner

   template <class Parser>
   typename SkipScanner <Parser>::type
   getSkipScanner (const Parser& parser)
   {
      return  typename SkipScanner <Parser>::type (*this, parser);
   }

   NoSkipScanner  getNoSkipScanner()
   {
      return  NoSkipScanner (*this);
   }

   UpperCaseScanner  getUpperCaseScanner()
   {
      return  UpperCaseScanner (*this);
   }

   NormalCaseScanner  getNormalCaseScanner()
   {
      return  NormalCaseScanner (*this);
   }


// Messages & ErrorInfo

   void addMessage (const std::string& where, unsigned code, 
                    const std::string& text, MSG_TYPE type = MSG_TYPE::ERROR)
   {
      if (base->error_pos == base->end_pos)
         base->error_info.addMessage (where, -1, code, text, type);
      else
         base->error_info.addMessage (where, base->error_pos.line, code, text, type);
   }


   void addMessage (const std::string& where, unsigned code = 0)
   {
      assert (base->error_pos != base->end_pos);
      base->error_info.addMessage (where, base->error_pos.line, code);
   }


   ErrorInfo&  getErrorInfo()
   {
      return base->error_info;
   }

   const ErrorInfo&  getErrorInfo() const
   {
      return base->error_info;
   }

   std::string  Context ()
   {
      if (empty ()) return  "at end of file";;

      static const unsigned  affix = 10;
      Range::iterator  pos = base->error_pos,
                       start = pos - affix;
      Range::iterator  stop  = pos + affix;

      if (start < base->start_pos)  start = base->start_pos;
      if (stop > base->end_pos)     stop = base->end_pos;

      std::string context;
      context.assign (start, pos);
      context += "^";
      if (pos < stop)  context.append (pos, stop);
      return  mgo::pretty_string (context);
   }

   void  matchFailure ()
   {
      if (base->isAtErrorPosition ()) return;

      //if (base->error_info.emptyRecord ())
      //   addMessage ("", 0, "Parsing failure at: " + Context ());
      base->keepErrorPosition ();
   }

   void newErrorRecord ()
   {
      base->error_info.startRecord ();
   }

   void ignoreMessages (bool value = true)
   {
      base->error_info.ignoreMessages (value);
   }


protected:
   mutable mgo::Wrapper<ScannerData<Range>, mgo::Assoc::AutoReference>    base;

   Skipper      skipfun;
   Transformer  transform;

private:
   Scanner&  operator= (const Scanner& other);
};


template <class Parser>
struct SkipScanner: public Scanner <mgo::Range<const char*>, SkipFun<Parser>>
{
   typedef  Scanner <mgo::Range<const char*>, SkipFun<Parser>>  Base;

   SkipScanner (const char* data): Base (data)  {}

   SkipScanner (const char* data, Parser& parser):
      Base (data, parser)  {}


   template <class Skipper, class Transf>
   SkipScanner (Scanner <mgo::Range<const char*>, Skipper, Transf>& scanner):
      Base (scanner)  {}

private:
   template <class Scanner>
   SkipScanner&  operator= (const Scanner& other);
};

//template <class Parser>
//using SkipScanner<Parser> =  Scanner<mgo::Range<const char*>, SkipFun<Parser>>;

} // namespace pi
#endif 


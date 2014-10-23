#ifndef  _ERRORINFO_H_
#define  _ERRORINFO_H_


#include <stack.h>
#include <util.h>
#include "Base.h"
#include <StrUtils.h>

namespace pi {


class  ErrorInfo
{
public:
   struct  StackEntry
   {
      StackEntry (size_t l, std::string& str):
         line(l), parser(str) {}

      size_t       line;
      std::string  parser;
   };

   typedef  mgo::Stack <StackEntry>  ErrorStack;



   ErrorInfo(): line_num(1)  {}

   template <class Parser>
   void addStackEntry (const Parser &parser)
   {
      std::stringstream  out;
      parser.toString (out, WRITEMODE::EXPAND);
      stack.push ( StackEntry (line_num, out.str()) );
   }

   template <class Parser>
   void addStackEntry (size_t pos, const Parser &parser)
   {
      std::stringstream  out;
      parser.toString (out, WRITEMODE::EXPAND);
      stack.push ( StackEntry (pos, out.str()) );
   }

   void  setMessage (const std::string& msg)
   {
      message = msg;
   }

   template <class Iter>
   void  setErrorPosition (Iter begin)
   {
      if (error_position.empty())
         error_position.assign (begin, begin + 10);
   }

   const std::string& getErrorPostion() const
   {
      return error_position;
   }


   const ErrorStack&  getErrorStack() const
   {
      return stack;
   }
   
   const std::string&  getMessage() const
   {
      return message;
   }

   size_t  getLine()
   {
      return  line_num;
   }


   bool  empty() const 
   {
      return  stack.empty();
   }


   void  clear()
   {
      error_position.clear();
      stack.clear();
   }

   void  processedChar (char ch)
   {
      TRC_DEBUG(parser) << "match: " << mgo::pretty_char(ch) << mgo::endl;
      if (ch == '\n') 
         ++line_num;
   }


   static  
   ErrorInfo&  Instance()
   {
      static  ErrorInfo  error_info;
      return  error_info;
   }

private:

   std::string  message;
   std::string  error_position;
   size_t       line_num;

   mgo::Stack <StackEntry>   stack;
};


std::ostream&  operator<< (std::ostream& out, ErrorInfo &error)
{
   out << "Parser error at '";
   mgo::pretty_print (out, error.getErrorPostion());
   out << "'" << std::endl << error.getMessage() << std::endl;

   const ErrorInfo::ErrorStack&  stack = error.getErrorStack();
   with (stack)
      out << "Line " << stack[pos_].line << ": Parser " << stack[pos_].parser << std::endl;

   return out;
}


} // namespace pi
#endif 


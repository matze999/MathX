#ifndef  _ERRORINFO_H_
#define  _ERRORINFO_H_


#include <util.h>
#include <meta/typetraits.h>
#include <meta/is_chararray.h>



enum class MSG_TYPE: char
{
   ERROR,
   WARNING,
   INFO,
};


inline
std::ostream&  operator<< (std::ostream& out, const MSG_TYPE type)
{
   return  out << (type == MSG_TYPE::ERROR?   "Error":
                   type == MSG_TYPE::WARNING? "Warning": "Info");
}




enum ErrorId
{
   PARSING_ERROR = 1000,
   FATAL_ERROR,
   INCOMPLETE_PARAMETER,

   INVALID_PARAMETER,
   PARAMETER_REQUIRED,
   UNDEF_PARAMETER,

   USER_ERROR
};



struct  Message
{
   Message ()  {}
   Message(const std::string& str, unsigned pos, int c, const std::string& cont, MSG_TYPE ty = MSG_TYPE::ERROR)
      : where(str), line (pos), code (c), text (cont), type (ty) {}

   Message(const std::string& str, unsigned pos, int c = 0)
      : where(str), line (pos), code (c), type (MSG_TYPE::ERROR) {}


   std::string  where;
   unsigned     line;
   unsigned     code;
   std::string  text;
   MSG_TYPE     type;

   void toStream (std::ostream& out) const
   {
      out << where; 
      if (line)  out << " (" << line << ')';
      out << ": " << type;
      if (code)  out << " " << code;
      if (!text.empty())  out << ": " << text;
   }
};


//Message  error (unsigned code, const std::string&  text)
//{
//   return  Message ("", 0, code, text, MSG_TYPE::ERROR);
//}
//Message  warning (unsigned code, const std::string&  text)
//{
//   return  Message ("", 0, code, text, MSG_TYPE::WARNING);
//}
//Message  info (unsigned code, const std::string&  text)
//{
//   return  Message ("", 0, code, text, MSG_TYPE::INFO);
//}



inline
std::ostream&  operator<< (std::ostream& out, const Message& msg)
{
   msg.toStream (out);
   return out;
}


//*** ErrorInfo

class  ErrorInfo: public std::vector <Message>
{
   // typedef  std::vector <Message>    MessageList;
public:
   ErrorInfo (): ignore_messages(false), stack_pos(0)  {}


   void addMessage (const std::string& where, unsigned line, unsigned code, 
                    const std::string& text, MSG_TYPE type = MSG_TYPE::ERROR)
   {
      if (ignore_messages)  return;

      emplace_back (where, line, code, text, type);
   }


   void addMessage (const std::string& where, unsigned line, unsigned code = 0)
   {
      if (ignore_messages)  return;

      emplace_back (where, line, code);
   }

   void addMessage (const Message& msg)
   {
      if (ignore_messages)  return;
      push_back (msg);
   }

   void ignoreMessages (bool value = true)
   {
      ignore_messages = value;
   }

   void clearRecord()
   {
      resize (stack_pos);
   }

   void startRecord ()
   {
      stack_pos = size();
   }

   bool  emptyRecord ()
   {
      return  stack_pos == size();
   }

   static  
   ErrorInfo&  Instance()
   {
      static  ErrorInfo  error_info;
      return  error_info;
   }

protected:
   //void clear ();
   bool  ignore_messages;
   unsigned stack_pos;
};



std::ostream&  operator<< (std::ostream& out, const ErrorInfo &error_info)
{
   for (auto msg: error_info)
      out << msg << std::endl;
   return out;
}


#endif 


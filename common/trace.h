#ifndef _MGO_TRACE_
#define _MGO_TRACE_

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <map>
#include <string>

#pragma warning (disable : 4250)  // 'class1' : inherits 'class2::member' via dominance 



#define   TL_NOTRACE  0
#define   TL_ERROR    1
#define   TL_WARNING  2
#define   TL_INFO     3
#define   TL_DEBUG    4


#define TRC_FILE(file)         mgo::Trace::Instance().setFile (file);

#define TRC_ERROR(section)     if (!mgo::Trace::Instance().isActive (1, #section)) {} \
                               else   mgo::Trace::Instance() (#section)
#define TRC_WARNING(section)   if (!mgo::Trace::Instance().isActive (2, #section)) {} \
                               else   mgo::Trace::Instance() (#section)


#define TRC_INFO(section)      if (!mgo::Trace::Instance().isActive (3, #section)) {} \
                                  else   mgo::Trace::Instance() (#section)
#define TRC_DEBUG(section)     if (!mgo::Trace::Instance().isActive (4, #section)) {} \
                                  else   mgo::Trace::Instance() (#section)

#define TRC_LEVEL                 mgo::Trace::TraceLevel()
#define TRC_COMPONENT(section)    mgo::Trace::Instance()[#section].level


#define TRC_MARK(entry)   mgo::TraceMarker _trc_marker_##__FILE__##__LINE__ (#entry);



#define  TRC_INDENT_SECTION(level, section)   \
   mgo::TraceIndentBlock  _section##_tb_

#define  TRC_INDENT(level, section)  \
   if (mgo::Trace::Instance().isActive (level, #section)) \
      if (mgo::TraceIndentBlock  tb)


#define  TRC_SECTION(level)   if (mgo::TraceSection _trace_section_=level)




namespace mgo {


struct TraceLine: public std::stringstream
{
   TraceLine(): level(TL_DEBUG)  {}

   TraceLine::TraceLine(const TraceLine &tl) 
   {
      level  = tl.level;
   } 

   int           level;
};



typedef std::string  TraceKey;


class Trace
{
public:
   Trace ()
   {
      indent = 0;
      trace_line = nullptr;
      out = &std::cout;
      is_line_writen = false;
   }

   // check if the the trace is active for the requested section
   bool  isActive (int level, TraceKey section)
   {
      return  level <= TraceLevel ()  &&  level <= map[section].level;
   }


   Trace&  operator() (const char *section)
   {
      if (section != trace_section)
      {
         if (!is_line_writen)  writeLine();
         trace_section = section;
         trace_line = &map[trace_section];
      }
      is_line_writen = false;
      return *this;
   }


   template <class T>
   Trace&  operator<< (const T &value)
   {
      *trace_line << value;
      return *this;
   }


   Trace&  operator<< (void (*fun) (Trace&))
   {
      fun (*this);   
      return *this;
   }


   
   TraceLine&   operator[] (TraceKey comp)            { return  map[comp]; }

   static 
   Trace&  Instance()
   {
      static Trace trace;
      return  trace;
   }


   static
   int&  TraceLevel()
   {
      static int level = 1;
      return level;
   }

   size_t        indent;

   void setFile (const char* name)
   {
      file_name = name;
      out = nullptr;
   }


   friend  void endl (Trace &trace)
   {
      trace.writeLine ();
   }


private:
   std::ostream &File()               
   {
      if (out == nullptr)
         out = new std::fstream (file_name, std::fstream::out);
      return *out; 
   }

   
   void  writeLine ()
   {
      if (trace_line == nullptr)  return;

      File() << trace_section << ": ";
      if (indent)
      {
         File() << "   ";
         for (size_t pos = 1; pos < indent; ++pos)
            File() << ".  ";
      }
      File() << trace_line->str() << std::endl;
      trace_line->str("");
      is_line_writen = true;
   }

   std::map <TraceKey, TraceLine>   map;
   std::ostream  *out;

   std::string   file_name;
   TraceLine    *trace_line;
   TraceKey      trace_section;
   bool          is_line_writen;
};




struct  TraceIndentBlock
{
   TraceIndentBlock ()              { ++Trace::Instance().indent; }
   ~TraceIndentBlock()              { --Trace::Instance().indent; }

   operator bool()                  { return true; }
};



struct TraceMarker
{
   TraceMarker (const char* txt): entry(txt)  
   {
       TRC_DEBUG (progress) << ">> " << entry << mgo::endl;
   }

   ~TraceMarker()
   {
       TRC_DEBUG (progress) << "<< " << entry << mgo::endl;
   }

private:
   const char* entry;
};



struct TraceSection
{
   TraceSection (int level)
   {
      default_level = TRC_LEVEL;
      TRC_LEVEL = (default_level < level? default_level: level);
   }

   ~TraceSection()
   {
      TRC_LEVEL = default_level;
   }

   operator bool()                  { return true; }
private:
   int  default_level;
};


} /* namespace mgo */
#endif  /* __TRACE_ */

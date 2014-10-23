#ifndef __TRACE_
#define __TRACE_

#include <iostream>
#include <sstream>
#include <map>
#include <string>
//#include "util.h"



//*** Variables MAX_TRACE_LEVEL has to be defined in your (base) header file



#ifndef MAX_TRACE_LEVEL
#define MAX_TRACE_LEVEL  4
#endif

#define TRC(section)           mgo::TraceInstance <true> (0) [#section].is_active
#define TRC_ERROR(section)     mgo::TraceInstance <1 <= MAX_TRACE_LEVEL> (1, #section)
#define TRC_WARNING(section)   mgo::TraceInstance <2 <= MAX_TRACE_LEVEL> (2, #section)
#define TRC_INFO(section)      mgo::TraceInstance <3 <= MAX_TRACE_LEVEL> (3, #section)
#define TRC_DEBUG(section)     mgo::TraceInstance <4 <= MAX_TRACE_LEVEL> (4, #section)



#define TRC_LEVEL     mgo::getTraceLevel()

#define  TRC_SECTION_INDENT(level, section)     mgo::TabifySection <level <= MAX_TRACE_LEVEL>  \
    section##ts (mgo::TraceInstance <true> (0)[#section].indent)

#define  TRC_INDENT(level, section)  \
   if (mgo::TabifySection <level <= MAX_TRACE_LEVEL> ts (mgo::TraceInstance <true> (0)[#section].indent))



namespace mgo {

struct  Tab
{
   Tab(): count(0)  {}

   Tab&  operator++()         { ++count;  return *this; }
   Tab&  operator--()         { --count;  return *this; }

   friend
   std::ostream&  operator<< (std::ostream& out, const Tab &tab)
   {
      for (size_t pos = 0; pos < tab.count; ++pos)  
        insertTab (out);
      return out;
   }

protected:
   static
   void insertTab (std::ostream& out)
   {
      std::cout << '.';
      for (size_t pos = 1; pos < TAB_SIZE; ++pos)  std::cout << ' ';
   }

   size_t  count;

   static const size_t  TAB_SIZE = 3;
};



struct TraceLine: public std::stringstream
{
   TraceLine(): is_active(true)  {}
   TraceLine (const TraceLine &)  {}

   Tab           indent;
   bool          is_active;
};



inline
int&  getTraceLevel ()
{
   static WRITEMODE level = WRITEMODE::EXPAND;
   return  level;
}



typedef std::string  TraceSection;


template <bool> class Trace
{
public:
   Trace ()
   {
      out = &std::cout;
   }



   Trace&  operator() (int level, TraceSection section)
   {
      is_trace_active = (level <= getTraceLevel()  &&  map[section].is_active);  
      trace_line = &map[section];
      trace_section = section;
      return *this;
   }


   template <class T>
   Trace&  operator<< (const T &value)
   {
      if (is_trace_active)  *trace_line << value;
      return *this;
   }


   Trace&  operator<< (void (*fun) (Trace&))
   {
      if (is_trace_active)   fun (*this);   
      return *this;
   }


   friend  void endl (Trace &trace);
   
   TraceLine&   operator[] (TraceSection comp)            { return  map[comp]; }

private:
   std::ostream &File()                { return *out; }

   
   void  writeLine ()
   {
      File() << trace_section << ": " << trace_line->indent << trace_line->str() << std::endl;
      trace_line->str("");
   }

   std::map <TraceSection, TraceLine>   map;
   TraceLine  *trace_line;
   int  record_level;
   bool  is_trace_active;

   TraceSection  trace_section;
   std::ostream  *out;
};



template<>  class Trace <false>
{
   Trace&  operator() (int, TraceSection) { return *this; }

   bool  isActive()                       { return  false; }

   template <class T>
   Trace&  operator<< (const T &value)    { return *this; }

   int operator[] (TraceSection)          { return 0; }
};



template <bool is_active>
Trace <is_active>  &TraceInstance (int level, const char *section = nullptr)
{
   static  Trace <is_active>  instance;
   return  section?  instance (level, section):  instance;
}

inline
void endl (Trace<true>  &trace)
{
   trace.writeLine ();
}



template <bool>
struct  TabifySection
{
   TabifySection (Tab &t): tab(t)         { ++tab; }
   ~TabifySection()                       { --tab; }

   operator bool()                        { return true; }

   Tab &tab;
};



template <>
struct  TabifySection <false>
{
   TabifySection (int)   {}

   operator bool()          { return true; }
};



} /* namespace mgo */
#endif  /* __TRACE_ */
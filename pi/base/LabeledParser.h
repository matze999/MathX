#ifndef _PARSER_LABEL
#define _PARSER_LABEL

#include "Base.h"


namespace pi { 
namespace _ {


template <class Parser>
struct label_p: public Parser
{
   label_p (const char* str): name(str)  {}

   template <class ...Args>
   label_p (const char* str, Args&&... args): Parser (args...), name(str)  {}


   void toString (std::ostream &out, WRITEMODE level = WRITEMODE::MINIMAL) const
   {
      if (level == WRITEMODE::MINIMAL || level == WRITEMODE::COMPACT)
         out << name;
      else
         child.toString (out, level);
   }

private:
    const char* name;
};

} // namespace _



template <class P>
BaseParser <_::label_p<P>>
Label (const char* name, const BaseParser<P> &lhs)
{
   return  BaseParser <_::label_p<P>> (name, lhs);
}


} // namespace pi 
#endif 


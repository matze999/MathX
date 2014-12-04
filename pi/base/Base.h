#ifndef  _PARSEUTIL_H_
#define  _PARSEUTIL_H_

#include <ostream>
#include <Trace.h>
//#include <Range.h>
#include <Expressions.h>
//#include <Listing.h>
#include <memory>



#define  BACK_TRACE   TRC_DEBUG (pi) << printExpression (*this) << mgo::endl;


namespace pi {



template <class Base, class type = typename  Base::value_type>
struct  add_plain_parser
{
   template <class Scanner>
   std::unique_ptr<type>  parse (Scanner& scanner) const
   {
      auto  attribute = std::make_unique <type> ();
      if (!static_cast<const Base*> (this)->parse (scanner, *attribute))
         attribute.release ();

      return attribute;
   }

};

enum ErrorId
{
   MATCH_FAILURE = 1000,
   PARSER_FAILURE,
   RULE_FAILURE,

   USER_WARNING = 2000,
   USER_ERROR,
};


template <class Scanner>
bool  MatchSuccess (Scanner& scanner, const std::string&  token)
{
   TRC_DEBUG (pi) << "match: " << token << mgo::endl;
   return true;
}


template <class Scanner>
bool  MatchFailure (Scanner& scanner)
{
   TRC_DEBUG (pi) << "no match" << mgo::endl;
   return false;
}


template <class Scanner, class Parser>
bool  MatchFailure (Scanner& scanner, const Parser&  parser)
{
   scanner.matchFailure ();
   ErrorInfo& info = scanner.getErrorInfo ();
   if (info.emptyRecord ())
   {
      scanner.addMessage (::toString (parser), MATCH_FAILURE, scanner.Context ());
   }
   return false;
}



template <class Scanner>
bool  RuleFailure (Scanner& scanner, const std::string&  name)
{
   scanner.matchFailure ();
   ErrorInfo& info = scanner.getErrorInfo ();
   if (info.emptyRecord ())
   {
      scanner.addMessage (name, RULE_FAILURE, scanner.Context ());
   }
   else if (info.back ().code != RULE_FAILURE)
   {
      scanner.addMessage (name, RULE_FAILURE);
   }
   return false;
}





namespace fn {

struct no_action
{
   template <class T>
   void operator() (const T &) const {}

   template <class T>
   void operator= (const T &) const {}
};

const no_action ignore;


} // namespace fn



} // namespace pi 
#endif

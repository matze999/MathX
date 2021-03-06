/*
Parser categories

Primitives
   eps         - always match, no input consumption
   nil         - newer match, no input consumption
   bool_       - match 'true' or 'false'
   char_       - match a character
   int_        - match a integer
   double_     - match a double value
   lit("abc")  - match string "abc" but ignore the result
   token("abc")- match string "abc"


   character classes
      char_["abc"]      - match any character in string [abc]
      char_['0', '9']   - match any character in range [0-9]

      ch::upper         - match any uppercase letters [A-Z]
      ch::lower         - match any lowercase letters [a-z]
      ch::alpha         - match alphabetic characters [a-zA-Z] 
      ch::alnum         - match word alphanumeric character [a-zA-Z0-9]
      ch::_             - match '_'
      ch::word          - match word charachter [a-zA-Z0-9_] 
      ch::digit         - match digit character [0-9]
                       
      ch::blank         - match [ \t]
      ch::space         - match [ \t\n\r]


   word        - match a whole word
               :=  ch::alnum >> *ch::word


Operators
   
   P >> Q      - parse P and than Q
   P | Q       - parse P or Q
   P / Q       - parse P but not Q
   P % Q       - list parser, match P any times (at least once) separated by Q 
               :=  P >> *(Q >> P)
   

   greedy operators (match as many as possible)
      -P       - optional parser, match P or null
      +P       - match P once or any times
      *P       - kleene star, match P any times, 0 - n times

   lazy operators (minimal match)
      P- Q     :=  -(P/Q) >> Q
      P+ Q     :=  +(P/Q) >> Q
      P* Q     :=  *(P/Q) >> Q

   P ^ Q       - parse P >> Q, P or Q
   (P, Q)      - options parser, parse one or both parser in any order

   !P          - match not P, ignore P result and do not consume any character from input
   ~P          - negation, match character not defined in character class P
   P[fun]      - if P matches, apply fun on the result of P to get a new result
   P(name)      - if P matches, assign the result to name.



Expressions

   Operator <Signatur>    - def a operator parser, which apply a function to the values before and after it
	

   Rule <Attribute, opt Scanner> - def a parser for an Attribut an for a specific Scanner

   Grammer <Attribute, SkipParser>
   ClassParser <T> - Base class for setup a own parser for class T
                     You have to implement the method 

                     template <class Scanner>
                     bool parseImp (Scanner scanner)

   ignore(P)    - match P but ignore its result
   no_skip(P)   - match P without using the skip parser in this context
   idle(P)      - match P but do not consume any character from input

   var(name)    - match always, use name as result
   ref(name)    - match always, use the reference to name as result

   _1           - Ref 1. Match the parser P stored as reference 1
   (_1 = P)     - match P and store P at reference 1
   (_1 == Q)    - match Q and check if the result is equal to Ref 1.

	Map <Parser, Value> - def a parser which replace its result with predefined values

   switch (P)   - match P, ignore its result. Switch the parsed value
   [
      case ('a') [Q1],        - case 'a' parse Q1
      case ('b') [Q2],        - case 'b' parse Q2
      default [Q3]            - default parse Q3
   ]
*/


#ifndef  _PARSER_H_
#define  _PARSER_H_

#include "primitive/Char.h"
#include "primitive/Charclass.h"
#include "primitive/Int.h"
#include "primitive/Literal.h"
#include "primitive/Word.h"
#include "primitive/Bool.h"
#include "primitive/Phrase.h"
#include "primitive/ValueParser.h"

#include "operator/Sequence.h"
#include "operator/Alternative.h"
#include "operator/Exclude.h"
#include "operator/List.h"
#include "operator/Repeat.h"
#include "operator/Lazy.h"
#include "operator/Option.h"
#include "operator/Not.h"
#include "operator/AndOr.h"

#include "expression/Rule.h"
#include "expression/Idle.h"
#include "expression/NoSkip.h"
#include "expression/Grammar.h"
#include "expression/Builder.h"
#include "expression/Symbol.h"
#include "expression/Map.h"
#include "expression/Switch.h"



namespace pi {



template <class Parser, class Scanner, class Value>
inline
bool  parse (Parser& parser, Scanner &scanner, Value& result)
{
   //static_assert0 (Parser::rank == PID::EXPRESSION  ||  Parser::rank == PID::RULE);
   return parser.parse (scanner, result);
}


template <class Parser, class Scanner, class Value>
inline
bool  parse (Parser& parser, Scanner &scanner, Value& result, ErrorInfo* error)
{

   error = &scanner.getErrorInfo();
   return parse (parser, scanner, result);
}


template <class Parser,class Value>
void  parse (Parser& parser, const char* input,  Value& result)
{
   Scanner<> scanner(input);
   if (!parse (parser, scanner, result))
      throw  scanner.getErrorInfo();
}

} // namespace pi
#endif  // _PARSER__

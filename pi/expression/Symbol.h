#ifndef  _PARSER_SYMBOL_H_
#define  _PARSER_SYMBOL_H_

#include "../base/Parser.h"
#include <fn.h>

namespace pi {

template <class P, class T>
auto  symbol (const BaseParser<P>& parser, const T& data) -> decltype (parser [mgo::fn::val (data)])
{
   return  parser [mgo::fn::val (data)];
}


template <class T>
auto  symbol (const char* key, const T& data) -> decltype (lit(key) [mgo::fn::val (data)])
{
   return  lit(key) [mgo::fn::val (data)];
}


//template <class P, class T>
//using  Symbol = BaseParser < _::functor_p < mgo::fn::value<T>, P > ;

} // namespace pi
#endif

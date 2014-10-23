#ifndef  _MAKEPARSER_H_
#define  _MAKEPARSER_H_

#include <util.h>
#include <meta/if.h>
#include <meta/is_chararray.h>
#include "../primitive/Literal.h"

namespace pi {
namespace mp {

using namespace ::mp;


struct UndefOperator {};



template <template <class, class, class> class Op, class P, class Q>
struct ParserParserOperator
{
   using type = BaseParser <Op <typename P::type, typename Q::type, void>>;

   static
   type  execute (const P &lhs, const Q &rhs)
   {
      return type (lhs, rhs);
   }
};


template <template <class, class, class> class Op, class P>
struct ParserLiteralOperator
{
   using type = BaseParser <Op <typename P::type, _::lit_p, void>>;

   static
   type  execute (const P &lhs, const char *rhs)
   {
      return type (lhs, _::lit_p (rhs));
   }
};


template <template <class, class, class> class Op, class Q>
struct LiteralParserOperator
{
   using type = BaseParser <Op <_::lit_p, typename Q::type, void>>;

   static
   type  execute (const char *lhs, const Q &rhs)
   {
      return type (_::lit_p (lhs), rhs);
   }
};



template <template <class, class, class> class Op, class P, class Q>
struct make_parser: public
   if_<and <is_parser<P>, is_parser<Q>>::value,    ParserParserOperator <Op, P, Q>,
   if_<and <is_parser<P>, is_chararray<Q>>::value, ParserLiteralOperator <Op, P>,
   if_<and <is_chararray<P>, is_parser<Q>>::value, LiteralParserOperator <Op, Q>,
                                                   UndefOperator>>>::type  {};

//template <class P>
//struct parser_id
//{
//   static const int value = (int) is_parser <P>::value +
//                            (int) is_base_of <operator_parser_tag, P>::value +
//                            (int) is_chararray <P>::value * 3;
//};
//
//template <class P, class Q, 
//          int left_id= parser_id<P>::value, 
//          int right_id= parser_id<Q>::value>
//struct  and_parser {};
//
//template <class P, class Q>
//struct  and_parser <P, Q, 1, 1>
//{
//   using base = _::and_p <typename P::type, typename Q::type>;
//   using type = BaseParser <base>;
//
//   static
//   type  execute (const P &lhs, const Q &rhs)
//   {
//      return type (lhs, rhs);
//   }
//};
//
//template <class P, class Q>
//struct  and_parser <P, Q, 3, 1>
//{
//   using base = _::and_p <_::lit_p, typename Q::type>;
//   using type = BaseParser <base>;
//
//   static
//   type  execute (const char* lhs, const Q &rhs)
//   {
//      return type (_::lit_p(lhs), rhs);
//   }
//};
//
//template <class P, class Q>
//struct  and_parser <P, Q, 1, 3>
//{
//   using base = _::and_p <typename P::type, _::lit_p>;
//   using type = BaseParser <base>;
//
//   static
//   type  execute (const char* lhs, const Q &rhs)
//   {
//      return type (lhs, _::lit_p(rhs));
//   }
//};
//

} // namespace mp
} // namespace pi
#endif

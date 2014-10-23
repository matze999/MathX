#ifndef  _PARSER_MAP_H_
#define  _PARSER_MAP_H_

#include "../base/Parser.h"



/* example


Mapping <int, std::string>  sym;
sym.add (4, "four")
	.add (5, "five")
	.add (6, "six")
	.add (7, "seven")
	.default_ ("number");

*/

namespace pi {
namespace _ {


template <class Parser, class Value>
struct map_p : public UnaryExpression < Parser >
{
	typedef  Value  value_type;
	typedef  typename Parser::value_type  Key;


	map_p (const Parser& parser = Parser()) : Super (parser), has_default(false)  {}


	template <class Scanner, class Attribute>
	bool  parse(Scanner &scanner, Attribute& attribute) const
	{
		Key key;
		if (!child.parse(scanner, key))  return false;

		if (symbols.count(key))
		{
			mp::assign(attribute, symbols[key]);
			return true;
		}

		if (has_default)
		{
			mp::assign(attribute, default_value);
			return true;
		}
		return false;
	}


	void toString(std::ostream &out, WRITEMODE level = WRITEMODE::EXPAND) const
	{
		out << "map<>";
	}


	map_p&  add (const Key& key, const Value& value)
	{
		symbols[key] = value;
		return *this;
	}

	map_p&  operator() (const Key& key, const Value& value)
	{
		return add (key, value);
	}

	void  default_(const Value& value)
	{
		default_value = value;
		has_default = true;
	}

private:
	mutable std::map <Key, Value>  symbols;
	Value  default_value;
	bool   has_default;
};

} // namespace _



template <class Parser, class Value>
using  Map = BaseParser <_::map_p <Parser, Value>>;

//template <class Parser, class Value>
//struct Map: public BaseParser <_::map_p <Parser, Value>>
//{
//	Map() = default;
//	Map (const Parser& parser): Super (parser) {}
//};

} // namespace pi
#endif

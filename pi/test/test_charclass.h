#include <util.h>

using namespace mgo;


void test_CharClass()
{

	auto  parser = +ch::word;

	assertParser (parser, "qwert", "qwert");
	assertParser (parser, "1234", "1234");
	assertParser (parser, "____", "____");


	std::string result;
	assertParserFailure (parser, "}asdf", result);
	assertParserFailure (parser, "+asdf", result);

}


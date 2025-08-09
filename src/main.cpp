#include <iostream>
#include <fstream>
#include <sstream>

#include <hryvnia_lang/Lexer.hpp>
#include <hryvnia_lang/Parser.hpp>
#include <hryvnia_lang/common.hpp>

int main()
{
	std::string source = R"(2

+

2)";
	std::istringstream sstream(source);

	Lexer lexer(sstream);
	
	auto lexemes = lexer.process();

	Parser p(lexemes);
	
	p.parse_primary();

	for (const auto& el : lexemes) {
		std::cout << el << std::endl;
	}


	return 0;
}
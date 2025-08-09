#include <iostream>
#include <fstream>
#include <sstream>

#include <hryvnia_lang/Lexer.hpp>
#include <hryvnia_lang/common.hpp>

int main()
{
	std::string source = R"(
		1
		2
		#!bin/bash
		#42 
		def
		extern
		#extern
		99
		#pipka popka
		3.14
		1.41421356237
		defextern
		extern
		9
)";
	std::istringstream sstream(source);

	Lexer Q(sstream);
	

	auto v = Q.process();

	for (const auto& el : v) {
		std::cout << el << std::endl;
	}

	return 0;
}
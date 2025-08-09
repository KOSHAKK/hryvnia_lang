#include <iostream>
#include <fstream>
#include <sstream>

#include "Lexer.hpp"
#include "common.hpp"


int main()
{
	std::string source = R"(
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
)";

	std::istringstream sstream(source);

	Lexer Q(sstream);
	
	Lexeme l;
	l = Q.get_next_token();

	while (l.token != Lexeme::Token::tok_eof)
	{
		std::cout << static_cast<int>(l.token) << " ";
		std::visit([](auto& val) {std::cout << val << std::endl; }, l.value);
		l = Q.get_next_token();
	}

	return 0;
}
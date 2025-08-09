#pragma once
#include <string>

#include "Lexeme.hpp"

class Lexer
{
public:
	explicit Lexer(std::istream& in);
	Lexeme get_next_token();

private:
	char get_next_char();

	std::string identifier_str{};
	double num_val{};
	int last_char{};
	std::istream& buffer;
};








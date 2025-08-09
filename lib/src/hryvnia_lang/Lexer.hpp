#pragma once
#include <string>
#include <vector>

#include "Lexeme.hpp"

class Lexer
{
public:
	explicit Lexer(std::istream& in);

	std::vector<Lexeme> process();

private:
	char get_next_char();
	Lexeme get_next_lexeme();

	std::string identifier_str{};
	double num_val{};
	int last_char{};
	std::istream& buffer;
};








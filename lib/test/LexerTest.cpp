#include <gtest/gtest.h>

#include <hryvnia_lang/Lexer.hpp>
#include <hryvnia_lang/Lexeme.hpp>
#include <hryvnia_lang/common.hpp>

#include <vector>

TEST(Lexer, Test1) {

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
	Lexer lex(sstream);

	auto processed = lex.process();
	

	std::vector<Lexeme> expected
	{
		{ Lexeme::Token::tok_number, 1.0 },
		{ Lexeme::Token::tok_number, 2.0 },
		{ Lexeme::Token::tok_def , "def"},
		{ Lexeme::Token::tok_extern , "extern"},
		{ Lexeme::Token::tok_number, 99.0 },
		{ Lexeme::Token::tok_number, 3.14 },
		{ Lexeme::Token::tok_number, 1.41421 },
		{ Lexeme::Token::tok_identifier , "defextern"},
		{ Lexeme::Token::tok_extern , "extern"},
		{ Lexeme::Token::tok_number, 9.0 },
		{ Lexeme::Token::tok_eof },
	};

	EXPECT_EQ(processed, expected);
}


TEST(Lexer, Test2) {

	std::string source = R"(
		2+2
		42-990.32
		5852*13+1
		432.22 < 101.22
)";

	std::istringstream sstream(source);
	Lexer lex(sstream);

	auto processed = lex.process();

	std::vector<Lexeme> expected
	{
		{ Lexeme::Token::tok_number, 2.0 },
		{ Lexeme::Token::tok_binop, "+" },
		{ Lexeme::Token::tok_number , 2.0 },
		{ Lexeme::Token::tok_number , 42.0 },
		{ Lexeme::Token::tok_binop, "-"},
		{ Lexeme::Token::tok_number, 990.32 },
		{ Lexeme::Token::tok_number, 5852.0 },
		{ Lexeme::Token::tok_binop, "*" },
		{ Lexeme::Token::tok_number , 13.0},
		{ Lexeme::Token::tok_binop, "+" },
		{ Lexeme::Token::tok_number, 1.0 },
		{ Lexeme::Token::tok_number, 432.22 },
		{ Lexeme::Token::tok_binop, "<" },
		{ Lexeme::Token::tok_number, 101.22 },
		{ Lexeme::Token::tok_eof },
	};

	EXPECT_EQ(processed, expected);
}

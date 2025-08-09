#pragma once
#include "Parser.hpp"

#include <memory>
#include <unordered_map>

#include <hryvnia_lang/Lexer.hpp>
#include <hryvnia_lang/AST.hpp>


class Parser
{
public:
	Parser(std::vector<Lexeme>& lexemes);

	std::unique_ptr<ExprAST> parse_number_expr();

	std::unique_ptr<ExprAST> parse_paren_expr();

	std::unique_ptr<ExprAST> parse_expr();

	std::unique_ptr<ExprAST> parse_identifier_expr();

	std::unique_ptr<ExprAST> parse_primary();

	std::unique_ptr<ExprAST> parse_bin_op_rhs(int expr_prec, std::unique_ptr<ExprAST> lhs);

	int get_tok_precedence();
private:
	std::vector<Lexeme>& lexemes;
	std::vector<Lexeme>::iterator curr_lexeme;

	std::unordered_map<std::string, int> binop_precedence;

};

#pragma once
#include "Parser.hpp"

#include <memory>
#include <unordered_map>

#include "Lexer.hpp"
#include "AST.hpp"


class Parser
{
public:
	Parser(std::vector<Lexeme>& lexemes);
	std::vector<ASTNode> parse();

private:
	std::vector<ASTNode> AST;
	std::vector<Lexeme>& lexemes;
	std::vector<Lexeme>::iterator curr_lexeme;
	std::unordered_map<std::string, int> binop_precedence;

	std::shared_ptr<ExprAST> parse_number_expr();
	std::shared_ptr<ExprAST> parse_paren_expr();
	std::shared_ptr<ExprAST> parse_expr();
	std::shared_ptr<ExprAST> parse_identifier_expr();
	std::shared_ptr<ExprAST> parse_primary();
	std::shared_ptr<ExprAST> parse_bin_op_rhs(int expr_prec, std::shared_ptr<ExprAST> lhs);
	std::shared_ptr<PrototypeAST> parse_prototype();
	std::shared_ptr<PrototypeAST> parse_extern();
	std::shared_ptr<FunctionAST> parse_definition();
	std::shared_ptr<FunctionAST> parse_top_level_expr();
	std::shared_ptr<ExprAST> parse_if_expr();
	void handle_definition();
	void handle_extern();
	void handle_top_level_expression();
	int get_tok_precedence();
};

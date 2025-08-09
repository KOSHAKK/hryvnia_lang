#include "Parser.hpp"
#include <hryvnia_lang/common.hpp>
#include <variant>

Parser::Parser(std::vector<Lexeme>& lexemes)
	: lexemes(lexemes),
	  curr_lexeme{ lexemes.begin() }
{
	binop_precedence["<"] = 10;
	binop_precedence["+"] = 20;
	binop_precedence["-"] = 20;
	binop_precedence["*"] = 40;
}

std::unique_ptr<ExprAST> Parser::parse_number_expr()
{
	auto result = std::make_unique<NumberExprAST>(std::get<double>(curr_lexeme->value));
	++curr_lexeme;
	return std::move(result);
	
}

std::unique_ptr<ExprAST> Parser::parse_paren_expr()
{
	++curr_lexeme;
	auto V = parse_expr();
	if (!V) {
		return nullptr;
	}

	if (std::get<std::string>(curr_lexeme->value) != ")") {
		return log_error("expected ')'");
	}
	++curr_lexeme;
	return V;
}

std::unique_ptr<ExprAST> Parser::parse_expr()
{
	auto lhs = parse_primary();
	if (!lhs)
		return nullptr;
	return parse_bin_op_rhs(0, std::move(lhs));
}

std::unique_ptr<ExprAST> Parser::parse_identifier_expr()
{
	std::string id_name = std::get<std::string>(curr_lexeme->value);

	++curr_lexeme;

	if (std::get<std::string>(curr_lexeme->value) != "(")
		return std::make_unique<VariableExprAST>(id_name);

	++curr_lexeme;
	std::vector<std::unique_ptr<ExprAST>> args;
	if (std::get<std::string>(curr_lexeme->value) != ")") {
		while (true) 
		{
			if (auto arg = parse_expr()) 
				args.push_back(std::move(arg));
			else
				return nullptr;
			
			if (std::get<std::string>(curr_lexeme->value) == ")")
				break;

			if (std::get<std::string>(curr_lexeme->value) != ",")
				return log_error("Expected ')' or ',' in argument list");
			++curr_lexeme;
		}
	}

	++curr_lexeme;

	return std::make_unique<CallExprAST>(id_name, std::move(args));
}

std::unique_ptr<ExprAST> Parser::parse_primary()
{
	switch (curr_lexeme->token)
	{
	case Lexeme::Token::tok_identifier:
		return parse_identifier_expr();
	case Lexeme::Token::tok_number:
		return parse_number_expr();
	case Lexeme::Token::tok_lparen:
		return parse_paren_expr();
	default:
		return log_error("unknown token when expecting an expression");
	}
}

std::unique_ptr<ExprAST> Parser::parse_bin_op_rhs(int expr_prec, std::unique_ptr<ExprAST> lhs)
{
	while (true) 
	{
		int tok_prec = get_tok_precedence();

		if (tok_prec < expr_prec)
			return lhs;

		std::string bin_op = std::get<std::string>(curr_lexeme->value);
		++curr_lexeme;
		
		auto rhs = parse_primary();

		if (!rhs) 
			return nullptr;

		int next_prec = get_tok_precedence();

		if (tok_prec < next_prec) {
			rhs = parse_bin_op_rhs(tok_prec + 1, std::move(rhs));
			if (!rhs)
				return nullptr;
		}
		lhs = std::make_unique<BinaryExprAST>(bin_op[0], std::move(lhs), std::move(rhs));
	}
}

int Parser::get_tok_precedence()
{
	if (curr_lexeme->token != Lexeme::Token::tok_binop)
		return -1;

	int tok_prec = binop_precedence[std::get<std::string>(curr_lexeme->value)];

	if (tok_prec <= 0) return -1;
	return tok_prec;
}

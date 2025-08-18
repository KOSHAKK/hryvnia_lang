#include "Parser.hpp"

#include <variant>

#include <hryvnia_lang/IRCtx.hpp>
#include <hryvnia_lang/common.hpp>
#include <hryvnia_lang/SpdLogWrapper.hpp>

#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"

Parser::Parser(std::vector<Lexeme>& lexemes)
	: lexemes(lexemes),
	  curr_lexeme{ lexemes.begin() }
{
	binop_precedence["<"] = 10;
	binop_precedence["+"] = 20;
	binop_precedence["-"] = 20;
	binop_precedence["*"] = 40;
}

std::shared_ptr<ExprAST> Parser::parse_number_expr()
{
	auto result = std::make_unique<NumberExprAST>(std::get<double>(curr_lexeme->value));
	++curr_lexeme;
	return std::move(result);
	
}

std::shared_ptr<ExprAST> Parser::parse_paren_expr()
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

std::shared_ptr<ExprAST> Parser::parse_expr()
{
	auto lhs = parse_primary();
	if (!lhs)
		return nullptr;
	return parse_bin_op_rhs(0, std::move(lhs));
}

std::shared_ptr<ExprAST> Parser::parse_identifier_expr()
{
	std::string id_name = std::get<std::string>(curr_lexeme->value);


	++curr_lexeme;
	
	if (!std::holds_alternative<std::string>(curr_lexeme->value)) {
		return std::make_unique<VariableExprAST>(id_name);
	}


	if (std::get<std::string>(curr_lexeme->value) != "(")
		return std::make_unique<VariableExprAST>(id_name);

	

	++curr_lexeme;
	std::vector<std::shared_ptr<ExprAST>> args;
	if (!std::holds_alternative<std::string>(curr_lexeme->value) || std::get<std::string>(curr_lexeme->value) != ")") {
		while (true) 
		{
			if (auto arg = parse_expr()) 
				args.push_back(std::move(arg));
			else
				return nullptr;
			
			if (std::holds_alternative<std::string>(curr_lexeme->value) && std::get<std::string>(curr_lexeme->value) == ")")
				break;

			if (!std::holds_alternative<std::string>(curr_lexeme->value) || std::get<std::string>(curr_lexeme->value) != ",")
				return log_error("Expected ')' or ',' in argument list");
			++curr_lexeme;
		}
	}

	++curr_lexeme;

	return std::make_unique<CallExprAST>(id_name, std::move(args));
}

std::shared_ptr<ExprAST> Parser::parse_primary()
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

std::shared_ptr<ExprAST> Parser::parse_bin_op_rhs(int expr_prec, std::shared_ptr<ExprAST> lhs)
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

std::shared_ptr<PrototypeAST> Parser::parse_prototype()
{
	if (curr_lexeme->token != Lexeme::Token::tok_identifier)
		return log_error_p("Expected function name in prototype");

	std::string fn_name = std::get<std::string>(curr_lexeme->value);
	++curr_lexeme;

	if (std::get<std::string>(curr_lexeme->value) != "(")
		return log_error_p("Expected '(' in prototype");

	std::vector<std::string> arg_names;
	while ((++curr_lexeme)->token == Lexeme::Token::tok_identifier)
		arg_names.push_back(std::get<std::string>(curr_lexeme->value));
	if (std::get<std::string>(curr_lexeme->value) != ")")
		return log_error_p("Expected ')' in prototype");

	// success.
	++curr_lexeme;

	return std::make_unique<PrototypeAST>(fn_name, std::move(arg_names));
}

std::shared_ptr<FunctionAST> Parser::parse_definition()
{
	++curr_lexeme;
	auto proto = parse_prototype();
	if (!proto) return nullptr;

	if (auto E = parse_expr())
		return std::make_unique<FunctionAST>(std::move(proto), std::move(E));
	return nullptr;
}

std::shared_ptr<PrototypeAST> Parser::parse_extern()
{
	++curr_lexeme;
	return parse_prototype();
}

std::shared_ptr<FunctionAST> Parser::parse_top_level_expr()
{
	if (auto E = parse_expr()) {
		auto Proto = std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::string>());
		return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
	}
	return nullptr;
}

void Parser::handle_definition()
{
	auto ptr = parse_definition();
	AST.push_back(ptr);
	if (ptr) {
		if (auto* FnIR = ptr->codegen()) {
			loger::info("Parse definition");
			//FnIR->print(llvm::errs());
			std::cout << std::endl;
			IRCtx::ExitOnErr(IRCtx::JIT->addModule(
				llvm::orc::ThreadSafeModule(std::move(IRCtx::module), std::move(IRCtx::context))));
			IRCtx::init();
		}
	}
	else {
		++curr_lexeme;
	}
}

void Parser::handle_extern()
{
	auto ptr = parse_extern();
	AST.push_back(ptr);
	if (ptr) {
		if (auto* FnIR = ptr->codegen()) {
			loger::info("Parse extern");
			//FnIR->print(llvm::errs());
			std::cout << std::endl;
			FunctionAST::function_protos[ptr->name] = std::move(ptr);
		}
	}
	else {
		++curr_lexeme;
	}
}

void Parser::handle_top_level_expression()
{
	auto ptr = parse_top_level_expr();
	AST.push_back(ptr);
	if (ptr) {
		if (auto* FnIR = ptr->codegen()) {

			loger::info("Parse top level expression");
			//FnIR->print(llvm::errs());
			std::cout << std::endl;

			auto RT = IRCtx::JIT->getMainJITDylib().createResourceTracker();

			auto TSM = llvm::orc::ThreadSafeModule(std::move(IRCtx::module), std::move(IRCtx::context));
			IRCtx::ExitOnErr(IRCtx::JIT->addModule(std::move(TSM), RT));

			IRCtx::init();

			auto expr_symbol = IRCtx::ExitOnErr(IRCtx::JIT->lookup("__anon_expr"));
			//assert(ExprSymbol && "Function not found");

			//if (!expr_symbol.getFlags()) {
			//	loger::error("Function not found");
			//}

			double (*FP)() = expr_symbol.getAddress().toPtr<double (*)()>();



			loger::info("Evaluated to {}", FP());	

			IRCtx::ExitOnErr(RT->remove());


		}
	}
	else {
		++curr_lexeme;
	}
}

std::vector<ASTNode> Parser::parse()
{
	while (true) {
		switch (curr_lexeme->token) {
		case Lexeme::Token::tok_eof:
			return AST;
		case Lexeme::Token::tok_semicol:
			++curr_lexeme;
			break;
		case Lexeme::Token::tok_def:
			handle_definition();
			break;
		case Lexeme::Token::tok_extern:
			handle_extern();
			break;
		default:
			handle_top_level_expression();
			break;
		}
	}
	return AST;
}

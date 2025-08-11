#pragma once
#include <iostream>
#include <hryvnia_lang/Lexer.hpp>
#include <hryvnia_lang/AST.hpp>
#include <memory>


template <typename... Ts>
struct match : Ts... {
	using Ts::operator()...;
};

template <typename... Ts>
match(Ts...) -> match<Ts...>;


std::ostream& operator<<(std::ostream& os, const std::monostate&);
std::ostream& operator<<(std::ostream& os, Lexeme::Token token);
std::ostream& operator<<(std::ostream& os, const Lexeme& lex);

//bool astnode_equal(const ASTNode& lhs, const ASTNode& rhs);

std::unique_ptr<ExprAST> log_error(const char* str);
std::unique_ptr<PrototypeAST> log_error_p(const char* str);



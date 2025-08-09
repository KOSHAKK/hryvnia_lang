#pragma once
#include <iostream>
#include <hryvnia_lang/Lexer.hpp>
#include <hryvnia_lang/AST.hpp>
#include <memory>


std::ostream& operator<<(std::ostream& os, const std::monostate&);
std::ostream& operator<<(std::ostream& os, Lexeme::Token token);
std::ostream& operator<<(std::ostream& os, const Lexeme& lex);

std::unique_ptr<ExprAST> log_error(const char* str);
std::unique_ptr<PrototypeAST> log_error_p(const char* str);
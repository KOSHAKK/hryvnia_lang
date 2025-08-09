#pragma once
#include <iostream>
#include <hryvnia_lang/Lexer.hpp>

std::ostream& operator<<(std::ostream& os, const std::monostate&) {
	return os << "<empty>";
}

std::ostream& operator<<(std::ostream& os, Lexeme::Token token) {
    switch (token) {
    case Lexeme::Token::tok_number:     os << "tok_number"; break;
    case Lexeme::Token::tok_identifier: os << "tok_identifier"; break;
    case Lexeme::Token::tok_def:        os << "tok_def"; break;
    case Lexeme::Token::tok_extern:     os << "tok_extern"; break;
    default:                           os << "unknown_token"; break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Lexeme& lex) {
    os << "{ token: " << lex.token << ", value: ";
    std::visit([](auto& value) { std::cout << value; }, lex.value);
    os << " }";
    return os;
}
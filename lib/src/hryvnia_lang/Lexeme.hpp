#pragma once
#include <variant>
#include <cmath>

struct Lexeme
{
    enum class Token
    {
        tok_eof,

        // commands
        tok_def,
        tok_extern,

        // primary
        tok_identifier,
        tok_number,

        tok_lparen,
        tok_rparen,

        tok_semicol,
        tok_comma,

        tok_binop,

        tok_undefine,
    };

    bool operator==(const Lexeme& other) const {
        if (token != other.token) return false;
        if (token == Token::tok_number) {
            double a = std::get<double>(value);
            double b = std::get<double>(other.value);
            return std::fabs(a - b) < 1e-5;
        }
        else {
            return value == other.value;
        }
    }

    Token token{};
    std::variant<std::monostate, std::string, double> value{ std::monostate{} };
};
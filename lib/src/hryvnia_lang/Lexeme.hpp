#pragma once
#include <variant>

struct Lexeme
{
    enum class Token
    {
        tok_eof = -1,

        // commands
        tok_def = -2,
        tok_extern = -3,

        // primary
        tok_identifier = -4,
        tok_number = -5,


        tok_undefine = 0,
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
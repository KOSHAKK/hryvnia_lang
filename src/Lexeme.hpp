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
    Token token{};
    std::variant<std::monostate, std::string, double> value{ std::monostate{} };
};
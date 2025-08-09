#include <fstream>
#include <sstream>

#include "Lexer.hpp"


Lexer::Lexer(std::istream& in)
    : buffer(in)
{ }

Lexeme Lexer::get_next_token()
{
    last_char = ' ';

    while (std::isspace(last_char))
        last_char = get_next_char();


    if (std::isalpha(last_char)) {
        identifier_str = last_char;
        while (std::isalnum((last_char = get_next_char())))
            identifier_str += last_char;

        if (identifier_str == "def")
            return { Lexeme::Token::tok_def, identifier_str };
        if (identifier_str == "extern")
            return { Lexeme::Token::tok_extern, identifier_str };
        return { Lexeme::Token::tok_identifier };
    }

    if (isdigit(last_char) || last_char == '.') {
        std::string num_str;
        do {
            num_str += last_char;
            last_char = get_next_char();
        } while (isdigit(last_char) || last_char == '.');

        num_val = strtod(num_str.c_str(), 0);
        return { Lexeme::Token::tok_number, num_val };

    }

    if (last_char == '#') { 
        do
            last_char = get_next_char();
        while (last_char != EOF && last_char != '\n' && last_char != '\r');

        if (last_char != EOF)
            return get_next_token();
    }

    if (last_char == EOF)
        return { Lexeme::Token::tok_eof };

    return { Lexeme::Token::tok_undefine };
}

char Lexer::get_next_char()
{
    char c;
    if (buffer.get(c))
        return c;
    else
        return EOF;
}

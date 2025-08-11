#include <fstream>
#include <sstream>
#include <iostream>

#include "Lexer.hpp"


Lexer::Lexer(std::istream& in)
    : buffer(in)
{ 
    last_char = ' ';
}

Lexeme Lexer::get_next_lexeme()
{
        while (std::isspace(last_char))
        last_char = get_next_char();

    if (last_char == '+' || last_char == '-' || last_char == '*' || last_char == '<') {
        std::string op;
        op.push_back(last_char);
        last_char = get_next_char();
        return { Lexeme::Token::tok_binop, std::move(op) };
    }

    if (last_char == '(') {
        last_char = get_next_char();
        return { Lexeme::Token::tok_lparen, "(" };
    }
    if (last_char == ')') {
        last_char = get_next_char();
        return { Lexeme::Token::tok_rparen, ")" };
    }

    if (last_char == ',') {
        last_char = get_next_char();
        return { Lexeme::Token::tok_comma, "," };
    }

    if (last_char == ';') {
        last_char = get_next_char();
        return { Lexeme::Token::tok_semicol, ";" };
    }

    if (std::isalpha(last_char)) {
        identifier_str = last_char;
        while (std::isalnum((last_char = get_next_char())))
            identifier_str += last_char;

        if (identifier_str == "def") {
            //last_char = get_next_char();
            return { Lexeme::Token::tok_def, identifier_str };
        }
        if (identifier_str == "extern") {
            //last_char = get_next_char();
            return { Lexeme::Token::tok_extern, identifier_str };
        }
        //last_char = get_next_char();
        return { Lexeme::Token::tok_identifier, identifier_str };
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
            return get_next_lexeme();
    }

    if (last_char == EOF)
        return { Lexeme::Token::tok_eof };

    return { Lexeme::Token::tok_undefine };
}

std::vector<Lexeme> Lexer::process()
{
    std::vector<Lexeme> v;
    
    Lexeme l;
    do {
        l = get_next_lexeme();
        v.push_back(l);
    } while (l.token != Lexeme::Token::tok_eof);

    return v;
}

char Lexer::get_next_char()
{
    char c;
    if (buffer.get(c))
        return c;
    else
        return EOF;
}

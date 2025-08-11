#include "common.hpp"

std::ostream& operator<<(std::ostream& os, const std::monostate&) {
    return os << "<empty>";
}

std::ostream& operator<<(std::ostream& os, Lexeme::Token token) {
    switch (token) {
    case Lexeme::Token::tok_number:     os << "tok_number"; break;
    case Lexeme::Token::tok_identifier: os << "tok_identifier"; break;
    case Lexeme::Token::tok_def:        os << "tok_def"; break;
    case Lexeme::Token::tok_extern:     os << "tok_extern"; break;
    case Lexeme::Token::tok_binop:      os << "tok_binop"; break;
    case Lexeme::Token::tok_lparen:     os << "tok_lparen"; break;
    case Lexeme::Token::tok_rparen:     os << "tok_rparen"; break;
    case Lexeme::Token::tok_eof:        os << "tok_eof"; break;
    case Lexeme::Token::tok_comma:      os << "tok_comma"; break;
    default:                            os << "unknown_token"; break;
    }
    return os;
}


//bool astnode_equal(const ASTNode& lhs, const ASTNode& rhs)
//{
//    if (lhs.index() != rhs.index())
//        return false;
//
//
//
//}


std::ostream& operator<<(std::ostream& os, const Lexeme& lex) {
    os << "{ token: " << lex.token << ", value: ";
    std::visit([&os](auto& value) { os << value; }, lex.value);
    os << " }";
    return os;
}

std::unique_ptr<ExprAST> log_error(const char* str) {
    fprintf(stderr, "Error: %s\n", str);
    return nullptr;
}

std::unique_ptr<PrototypeAST> log_error_p(const char* str) {
    log_error(str);
    return nullptr;
}


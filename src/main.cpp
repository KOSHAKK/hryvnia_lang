#include <iostream>
#include <fstream>
#include <sstream>

#include <hryvnia_lang/Lexer.hpp>
#include <hryvnia_lang/Parser.hpp>
#include <hryvnia_lang/AST.hpp>
#include <hryvnia_lang/common.hpp>

int main()
{
	std::string source = R"(

		def foo2(y) foo1(y, foo3(5.0));
		def foo(x y) x+foo1(y, 3.0);
		extern sin(a);
		extern pow(a b);
		def foo(x y) x+foo(y, 4.0)
		def foo(x y) x+y y;
		def foo(x y) x*y;
		2 2;

)";
	std::istringstream sstream(source);

	Lexer lexer(sstream);
	
	auto lexemes = lexer.process();
	lexemes.push_back({ Lexeme::Token::tok_eof });


	Parser p(lexemes);

	p.parse();

	//std::vector<ExprAST>
	

	//for (const auto& el : lexemes) {
	//	std::cout << el << std::endl;
	//}


	return 0;
}
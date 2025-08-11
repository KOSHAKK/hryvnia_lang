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
		

		def foo(x y) x+y;

)";
	std::istringstream sstream(source);

	Lexer lexer(sstream);

	auto lexemes = lexer.process();


	Parser p(lexemes);

	auto v = p.parse();




	ASTNode f = v[0];


	BinaryExprAST bin('+', std::make_shared<VariableExprAST>("x"), std::make_shared<VariableExprAST>("y"));
	PrototypeAST prot("foo", std::vector<std::string>{"x", "y"});
	FunctionAST my_f(std::make_shared<PrototypeAST>(prot), std::make_shared<BinaryExprAST>(bin));

	ASTNode my_node = std::make_shared<FunctionAST>(my_f);

	std::cout << (f == my_node) << std::endl;

	return 0;
}
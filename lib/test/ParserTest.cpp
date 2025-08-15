#include <gtest/gtest.h>

#include <hryvnia_lang/Lexer.hpp>
#include <hryvnia_lang/Lexeme.hpp>
#include <hryvnia_lang/Parser.hpp>
#include <hryvnia_lang/AST.hpp>
#include <hryvnia_lang/common.hpp>
#include <hryvnia_lang/IRCtx.hpp>


#include <vector>

TEST(Parser, Test1) {
	IRCtx cont;
	cont.init();

	std::string source = R"(

		extern pow(a b);
		extern sin(a);

)";
	std::istringstream sstream(source);

	Lexer lexer(sstream);
	auto lexemes = lexer.process();


	Parser p(lexemes);
	auto processed = p.parse();

	std::vector<ASTNode> expected;
	expected.push_back(std::make_shared<PrototypeAST>("pow", std::vector<std::string>{"a", "b"}));
	expected.push_back(std::make_shared<PrototypeAST>("sin", std::vector<std::string>{"a"}));


	EXPECT_EQ(expected, processed);
}

TEST(Parser, Test2) {

	std::string source = R"(

		def foo(x y) x+y;
		def zxc(x y z) x*z;

)";
	std::istringstream sstream(source);

	Lexer lexer(sstream);
	auto lexemes = lexer.process();


	Parser p(lexemes);
	auto processed = p.parse();

	std::vector<ASTNode> expected;
	BinaryExprAST bin_expr1('+', std::make_shared<VariableExprAST>("x"), std::make_shared<VariableExprAST>("y"));
	PrototypeAST prot1("foo", std::vector<std::string>{"x", "y"});
	FunctionAST expected_f1(std::make_shared<PrototypeAST>(prot1), std::make_shared<BinaryExprAST>(bin_expr1));

	expected.push_back(std::make_shared<FunctionAST>(expected_f1));


	BinaryExprAST bin_expr2('*', std::make_shared<VariableExprAST>("x"), std::make_shared<VariableExprAST>("z"));
	PrototypeAST prot2("zxc", std::vector<std::string>{"x", "y", "z"});
	FunctionAST expected_f2(std::make_shared<PrototypeAST>(prot2), std::make_shared<BinaryExprAST>(bin_expr2));

	expected.push_back(std::make_shared<FunctionAST>(expected_f2));

	EXPECT_EQ(expected, processed);
}

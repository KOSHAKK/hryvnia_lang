#include <iostream>
#include <fstream>
#include <sstream>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include <hryvnia_lang/Lexer.hpp>
#include <hryvnia_lang/Parser.hpp>
#include <hryvnia_lang/AST.hpp>
#include <hryvnia_lang/common.hpp>
#include <hryvnia_lang/IRCtx.hpp>
#include <hryvnia_lang/SpdLogWrapper.hpp>

#include "llvm/Support/TargetSelect.h"


int main()
{
	spdlog::set_pattern("%^[%l]%$ %v");


	IRCtx::init_target_and_jit();
	IRCtx::init();

	std::string source = R"(
			
		extern sin(x);
		extern cos(x);
		extern pow(a b);

		def foo(x) sin(x)*sin(x) + cos(x)*cos(x);

		foo(42);		

		pow(12,2);
		

)";
	std::istringstream sstream(source);

	Lexer lexer(sstream);

	auto lexemes = lexer.process();


	Parser p(lexemes);

	auto v = p.parse();


	return 0;
}





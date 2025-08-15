#include "IRCtx.hpp"

std::unique_ptr<llvm::LLVMContext> IRCtx::context;
std::unique_ptr<llvm::IRBuilder<>> IRCtx::builder;
std::unique_ptr<llvm::Module> IRCtx::module;
std::map<std::string, llvm::Value*> IRCtx::named_values;

void IRCtx::init()
{
	// Open a new context and module.
	context = std::make_unique<llvm::LLVMContext>();
	module = std::make_unique<llvm::Module>("hryvnia lang JIT", *context);

	// Create a new builder for the module.
	builder = std::make_unique<llvm::IRBuilder<>>(*context);
}
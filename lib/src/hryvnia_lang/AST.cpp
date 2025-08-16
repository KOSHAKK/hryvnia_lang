#include "AST.hpp"

#include <hryvnia_lang/common.hpp>
#include <hryvnia_lang/IRCtx.hpp>
#include <llvm/IR/Verifier.h>

bool ASTNode_equals(const ASTNode& lhs, const ASTNode& rhs)
{
	if (lhs.index() != rhs.index())
		return false;

	return std::visit(match{
		[](const std::nullptr_t& lhs, const std::nullptr_t& rhs) { return true; },
		[](const std::nullptr_t& lhs, auto&) { return false; },
		[](auto&, const std::nullptr_t& lhs) { return false; },
		[](const std::shared_ptr<ExprAST> lhs, const std::shared_ptr<ExprAST> rhs) { return lhs->equals(*rhs); },
		[](const std::shared_ptr<FunctionAST> lhs, const std::shared_ptr< FunctionAST> rhs) { return lhs->equals(*rhs); },
		[](const std::shared_ptr<PrototypeAST> lhs, const std::shared_ptr<PrototypeAST> rhs) { return lhs->equals(*rhs); },
		[](auto&, auto&) { return false; },
	}, lhs, rhs);
		
}

bool operator==(const ASTNode& lhs, const ASTNode& rhs)
{
	return ASTNode_equals(lhs, rhs);
}

llvm::Value* NumberExprAST::codegen()
{
	return llvm::ConstantFP::get(*IRCtx::context, llvm::APFloat(val));
}

llvm::Value* VariableExprAST::codegen()
{
	llvm::Value* V = IRCtx::named_values[name];
	if (!V)
		log_error_v("Unknown variable name");
	return V;
}

llvm::Value* BinaryExprAST::codegen()
{
	llvm::Value* L = lhs->codegen();
	llvm::Value* R = rhs->codegen();
	if (!L || !R)
		return nullptr;

	switch (op) {
	case '+':
		return IRCtx::builder->CreateFAdd(L, R, "addtmp");
	case '-':
		return IRCtx::builder->CreateFSub(L, R, "subtmp");
	case '*':
		return IRCtx::builder->CreateFMul(L, R, "multmp");
	case '<':
		L = IRCtx::builder->CreateFCmpULT(L, R, "cmptmp");
		// Convert bool 0/1 to double 0.0 or 1.0
		return IRCtx::builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*IRCtx::context),
			"booltmp");
	default:
		return log_error_v("invalid binary operator");
	}
}

llvm::Value* CallExprAST::codegen()
{
	// Look up the name in the global module table.
	llvm::Function* CalleeF = IRCtx::module->getFunction(callee);
	if (!CalleeF)
		return log_error_v("Unknown function referenced");

	// If argument mismatch error.
	if (CalleeF->arg_size() != args.size())
		return log_error_v("Incorrect # arguments passed");

	std::vector<llvm::Value*> args_v;
	for (unsigned i = 0, e = args.size(); i != e; ++i) {
		args_v.push_back(args[i]->codegen());
		if (!args_v.back())
			return nullptr;
	}

	return IRCtx::builder->CreateCall(CalleeF, args_v, "calltmp");
}

llvm::Function* PrototypeAST::codegen()
{
	// Make the function type:  double(double,double) etc.
	std::vector<llvm::Type*> Doubles(args.size(),
		llvm::Type::getDoubleTy(*IRCtx::context));
	llvm::FunctionType* FT =
		llvm::FunctionType::get(llvm::Type::getDoubleTy(*IRCtx::context), Doubles, false);

	llvm::Function* F =
		llvm::Function::Create(FT, llvm::Function::ExternalLinkage, name, IRCtx::module.get());

	// Set names for all arguments.
	unsigned idx = 0;
	for (auto& arg : F->args())
		arg.setName(args[idx++]);

	return F;
}

llvm::Function* FunctionAST::codegen()
{
	// First, check for an existing function from a previous 'extern' declaration.
	llvm::Function* fn = IRCtx::module->getFunction(proto->name);

	if (!fn)
		fn = proto->codegen();

	if (!fn)
		return nullptr;

	if (!fn->empty())
		return static_cast<llvm::Function*>(log_error_v("Function cannot be redefined."));

	// Create a new basic block to start insertion into.
	llvm::BasicBlock* BB = llvm::BasicBlock::Create(*IRCtx::context, "entry", fn);
	IRCtx::builder->SetInsertPoint(BB);

	// Record the function arguments in the NamedValues map.
	IRCtx::named_values.clear();
	for (auto& arg : fn->args())
		IRCtx::named_values[std::string(arg.getName())] = &arg;
	
	if (llvm::Value* RetVal = body->codegen()) {
		// Finish off the function.
		IRCtx::builder->CreateRet(RetVal);

		// Validate the generated code, checking for consistency.
		llvm::verifyFunction(*fn);
		
		IRCtx::FPM->run(*fn, *IRCtx::FAM);

		return fn;
	}

	// Error reading body, remove function.
	fn->eraseFromParent();
	return nullptr;
}

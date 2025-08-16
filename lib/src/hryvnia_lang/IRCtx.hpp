#pragma once

#include <memory>
#include <map>
#include <string>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/PassInstrumentation.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Passes/StandardInstrumentations.h"

#include <hryvnia_lang/KaleidoscopeJIT.h>

class IRCtx
{
public:
	static void init();
	static void init_target_and_jit();


	static std::unique_ptr<llvm::LLVMContext> context;
	static std::unique_ptr<llvm::IRBuilder<>> builder;
	static std::unique_ptr<llvm::Module> module;
	static std::map<std::string, llvm::Value*> named_values;

	static std::unique_ptr<llvm::FunctionPassManager> FPM;
	static std::unique_ptr<llvm::LoopAnalysisManager> LAM;
	static std::unique_ptr<llvm::FunctionAnalysisManager> FAM;
	static std::unique_ptr<llvm::CGSCCAnalysisManager> CGAM;
	static std::unique_ptr<llvm::ModuleAnalysisManager> MAM;
	static std::unique_ptr<llvm::PassInstrumentationCallbacks> PIC;
	static std::unique_ptr<llvm::StandardInstrumentations> SI;

	static std::unique_ptr<llvm::orc::KaleidoscopeJIT> JIT;

	static llvm::ExitOnError ExitOnErr;

};


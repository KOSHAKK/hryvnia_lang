#include "IRCtx.hpp"

#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/TargetSelect.h"

#include "KaleidoscopeJIT.h"

std::unique_ptr<llvm::LLVMContext> IRCtx::context;
std::unique_ptr<llvm::IRBuilder<>> IRCtx::builder;
std::unique_ptr<llvm::Module> IRCtx::module;
std::map<std::string, llvm::Value*> IRCtx::named_values;

std::unique_ptr<llvm::FunctionPassManager> IRCtx::FPM;
std::unique_ptr<llvm::LoopAnalysisManager> IRCtx::LAM;
std::unique_ptr<llvm::FunctionAnalysisManager> IRCtx::FAM;
std::unique_ptr<llvm::CGSCCAnalysisManager> IRCtx::CGAM;
std::unique_ptr<llvm::ModuleAnalysisManager> IRCtx::MAM;
std::unique_ptr<llvm::PassInstrumentationCallbacks> IRCtx::PIC;
std::unique_ptr<llvm::StandardInstrumentations> IRCtx::SI;

std::unique_ptr<llvm::orc::KaleidoscopeJIT> IRCtx::JIT;

llvm::ExitOnError IRCtx::ExitOnErr;



void IRCtx::init()
{
	// Open a new context and module.
	context = std::make_unique<llvm::LLVMContext>();
	module = std::make_unique<llvm::Module>("hryvnia lang JIT", *context);
	module->setDataLayout(JIT->getDataLayout());

	// Create a new builder for the module.
	builder = std::make_unique<llvm::IRBuilder<>>(*context);

	// Create new pass and analysis managers.
	FPM = std::make_unique<llvm::FunctionPassManager>();
	LAM = std::make_unique<llvm::LoopAnalysisManager>();
	FAM = std::make_unique<llvm::FunctionAnalysisManager>();
	CGAM = std::make_unique<llvm::CGSCCAnalysisManager>();
	MAM = std::make_unique<llvm::ModuleAnalysisManager>();
	PIC = std::make_unique<llvm::PassInstrumentationCallbacks>();
	SI = std::make_unique<llvm::StandardInstrumentations>(*context,
		/*DebugLogging*/ true);
	SI->registerCallbacks(*PIC, MAM.get());

	// Add transform passes.
// Do simple "peephole" optimizations and bit-twiddling optzns.
	FPM->addPass(llvm::InstCombinePass());
	// Reassociate expressions.
	FPM->addPass(llvm::ReassociatePass());
	// Eliminate Common SubExpressions.
	FPM->addPass(llvm::GVNPass());
	// Simplify the control flow graph (deleting unreachable blocks, etc).
	FPM->addPass(llvm::SimplifyCFGPass());

	// Register analysis passes used in these transform passes.
	llvm::PassBuilder PB;
	PB.registerModuleAnalyses(*MAM);
	PB.registerFunctionAnalyses(*FAM);
	PB.crossRegisterProxies(*LAM, *FAM, *CGAM, *MAM);
}

void IRCtx::init_target_and_jit()
{
	llvm::InitializeNativeTarget();
	llvm::InitializeNativeTargetAsmPrinter();
	llvm::InitializeNativeTargetAsmParser();

	JIT = ExitOnErr(llvm::orc::KaleidoscopeJIT::Create());
}

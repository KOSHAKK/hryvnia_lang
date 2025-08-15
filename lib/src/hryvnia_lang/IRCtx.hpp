#pragma once

#include <memory>
#include <map>
#include <string>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"


class IRCtx
{
public:
	static void init();

	static std::unique_ptr<llvm::LLVMContext> context;
	static std::unique_ptr<llvm::IRBuilder<>> builder;
	static std::unique_ptr<llvm::Module> module;
	static std::map<std::string, llvm::Value*> named_values;
};


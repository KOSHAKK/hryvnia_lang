#pragma once
#include <string>
#include <memory>
#include <vector>
#include <variant>
#include <cmath>

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"


class ExprAST {
public:
    virtual ~ExprAST() = default;
    virtual bool equals(const ExprAST& other) const = 0;
    virtual llvm::Value* codegen() = 0;
};

class NumberExprAST : public ExprAST {
public:
    double val;
    NumberExprAST(double v) : val(v) {}
    bool equals(const ExprAST& other) const override {
        if (auto p = dynamic_cast<const NumberExprAST*>(&other))
            return std::fabs(val - p->val) < 1e-5;
        return false;
    }

    llvm::Value* codegen() override;
};

class VariableExprAST : public ExprAST {
public:
    std::string name;
    VariableExprAST(const std::string& n) : name(n) {}
    bool equals(const ExprAST& other) const override {
        if (auto p = dynamic_cast<const VariableExprAST*>(&other))
            return name == p->name;
        return false;
    }

    llvm::Value* codegen() override;
};

class BinaryExprAST : public ExprAST {
public:
    char op;
    std::shared_ptr<ExprAST> lhs, rhs;
    BinaryExprAST(char op_, std::shared_ptr<ExprAST> L, std::shared_ptr<ExprAST> R)
        : op(op_), lhs(std::move(L)), rhs(std::move(R)) {
    }
    bool equals(const ExprAST& other) const override {
        if (auto p = dynamic_cast<const BinaryExprAST*>(&other)) {
            if (op != p->op) return false;
            if (!lhs && !p->lhs && !rhs && !p->rhs) return true;
            if (!lhs || !p->lhs || !rhs || !p->rhs) return false;
            return lhs->equals(*p->lhs) && rhs->equals(*p->rhs);
        }
        return false;
    }

    llvm::Value* codegen() override;
};

class CallExprAST : public ExprAST {
public:
	std::string callee;
	std::vector<std::shared_ptr<ExprAST>> args;

	CallExprAST(const std::string& callee,
		std::vector<std::shared_ptr<ExprAST>> args)
		: callee(callee), args(std::move(args)) {
	}
    bool equals(const ExprAST& other) const override {
        if (auto p = dynamic_cast<const CallExprAST*>(&other)) {
            if ((callee != p->callee) || args.size() != p->args.size()) return false;
            for (int i = 0; i < callee.size(); i++) {
                if (!args[i]->equals(*(p->args[i])))
                    return false;
            }
            return true;
        }
        return false;
    }

    llvm::Value* codegen() override;
};


class PrototypeAST {
public:
	std::string name;
	std::vector<std::string> args;

	PrototypeAST(const std::string& name, std::vector<std::string> args)
		: name(name), args(std::move(args)) {
	}
    bool equals(const PrototypeAST& other) const {
        return name == other.name && args == other.args;
    }

    llvm::Function* codegen();
};


class FunctionAST {
public:
	std::shared_ptr<PrototypeAST> proto;
	std::shared_ptr<ExprAST> body;

	FunctionAST(std::shared_ptr<PrototypeAST> proto,
		std::shared_ptr<ExprAST> body)
		: proto(std::move(proto)), body(std::move(body)) {
	}
    bool equals(const FunctionAST& other) const {
        if (!proto->equals(*other.proto)) return false;
        if (!body->equals(*other.body)) return false;
        return true;
    }

    llvm::Function* codegen();
};

using ASTNode = std::variant<std::shared_ptr<ExprAST>, std::shared_ptr<FunctionAST>, std::shared_ptr<PrototypeAST>, std::nullptr_t>;


bool ASTNode_equals(const ASTNode& lhs, const ASTNode& rhs);

bool operator==(const ASTNode& lhs, const ASTNode& rhs);

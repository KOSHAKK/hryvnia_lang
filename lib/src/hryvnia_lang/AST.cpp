#include "AST.hpp"

#include <hryvnia_lang/common.hpp>

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

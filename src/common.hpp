#pragma once
#include <iostream>

std::ostream& operator<<(std::ostream& os, const std::monostate&) {
	return os << "<empty>";
}
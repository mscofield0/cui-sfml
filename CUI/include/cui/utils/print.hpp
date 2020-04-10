#ifndef CUI_PRINT_HPP
#define CUI_PRINT_HPP

#include <iostream>

template <typename... Args>
void print(Args&&... args) {
	((std::cout << args << ' '), ...);
}

template <typename... Args>
void println(Args&&... args) {
	((std::cout << args << ' '), ...);
	std::cout << '\n';
}

#endif	  // CUI_PRINT_HPP
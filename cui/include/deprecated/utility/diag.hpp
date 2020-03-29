#ifndef CUI_DIAG_HPP
#define CUI_DIAG_HPP

#include <fstream>
#include <iostream>
#include <functional>

namespace cui {

void dump(std::string const& filename, std::function<std::string()>&& func) {
	auto s = std::ofstream{filename};
	s << func();
}

}	 // namespace cui

#endif	  //CUI_DIAG_HPP

#ifndef CUI_DATA_TYPES_STRING_HPP
#define CUI_DATA_TYPES_STRING_HPP

#include <string>

#include <compile_time/string/string_view.hpp>

namespace cui::data_types {

class String
{
public:
	String(const ct::StringView str) : str_(str.begin(), str.end()) {}
	explicit String(const std::string& str) : str_(str) {}

	[[nodiscard]] operator const std::string &() const {
		return str_;
	}

private:
	std::string str_;
};

}	 // namespace cui::data_types

#endif	  // CUI_DATA_TYPES_STRING_HPP
#ifndef CUI_SFML_CONVERSION_ERROR_HPP
#define CUI_SFML_CONVERSION_ERROR_HPP

#include <exception>

namespace cui {

class ConversionError final : public std::logic_error {
	using std::logic_error::logic_error;
};

}

#endif	  // CUI_SFML_CONVERSION_ERROR_HPP
#ifndef CUI_SFML_FLOOR_HPP
#define CUI_SFML_FLOOR_HPP

namespace cui {

constexpr float floor(float num) noexcept {
	return static_cast<float>(static_cast<int>(num));
}

}

#endif // CUI_SFML_FLOOR_HPP
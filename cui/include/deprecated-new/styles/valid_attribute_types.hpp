#ifndef CUI_STYLES_VALID_ATTRIBUTE_TYPES_HPP
#define CUI_STYLES_VALID_ATTRIBUTE_TYPES_HPP

#include <global/constants.hpp>

namespace cui::styles {

class ValidAttributeType
{
public:
	constexpr ValidAttributeType(uint64_t p_x = 0) : x(p_x) {}

	constexpr operator uint64_t() const {
		return x;
	}

	static const ValidAttributeType Invalid, RGBA, Vec2, Vec3, Vec4, Float, Int, Instruction;

private:
	uint64_t x;
};

// clang-format off
constexpr const ValidAttributeType ValidAttributeType::Invalid    {1 << 0},
                                   ValidAttributeType::RGBA       {1 << 1},
                                   ValidAttributeType::Vec2       {1 << 2},
                                   ValidAttributeType::Vec3       {1 << 3},
                                   ValidAttributeType::Vec4       {1 << 4},
                                   ValidAttributeType::Float      {1 << 5},
                                   ValidAttributeType::Int        {1 << 6},
                                   ValidAttributeType::Instruction{1 << 7};
// clang-format on

constexpr auto operator|(const ValidAttributeType& a, const ValidAttributeType& b) -> uint64_t {
	return static_cast<uint64_t>(a) | static_cast<uint64_t>(b);
}

}	 // namespace cui::styles

#endif	  // CUI_STYLES_VALID_ATTRIBUTE_TYPES_HPP
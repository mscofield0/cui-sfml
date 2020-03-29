#ifndef CUI_STYLES_DATA_HPP
#define CUI_STYLES_DATA_HPP

#include <global/empty_type.hpp>
#include <data_types/color.hpp>
#include <data_types/vector.hpp>
#include <data_types/instruction.hpp>

namespace cui::styles {

enum class DataTypes : uint8_t
{
	None = 0,
	Color,
	Float,
	Int,
	Vector2,
	Vector3,
	Vector4,
	Instruction
};

class ValueData
{
public:
	constexpr ValueData() : active_(DataTypes::None), none_() {}
	constexpr ValueData(const Color& color) : active_(DataTypes::Color), rgba_(color) {}
	constexpr ValueData(const float val) : active_(DataTypes::Float), float_value_(val) {}
	constexpr ValueData(const int val) : active_(DataTypes::Int), integer_value_(val) {}
	constexpr ValueData(const Vector2f& v2) : active_(DataTypes::Vector2), vec2_(v2) {}
	constexpr ValueData(const Vector3f& v3) : active_(DataTypes::Vector3), vec3_(v3) {}
	constexpr ValueData(const Vector4f& v4) : active_(DataTypes::Vector4), vec4_(v4) {}
	constexpr ValueData(const Instruction& instr) : active_(DataTypes::Instruction), instruction_(instr) {}
	constexpr ValueData(const ValueData& other) = default;

	constexpr ValueData& operator=(const ValueData& a) {
		active_ = a.active_;
		switch (a.active_) {
			case DataTypes::None: {
				return (*this);
			}
			case DataTypes::Color: {
				rgba_ = a.rgba();
				return (*this);
			}
			case DataTypes::Float: {
				float_value_ = a.float_value();
				return (*this);
			}
			case DataTypes::Int: {
				integer_value_ = a.integer_value();
				return (*this);
			}
			case DataTypes::Vector2: {
				vec2_ = a.vector2();
				return (*this);
			}
			case DataTypes::Vector3: {
				vec3_ = a.vector3();
				return (*this);
			}
			case DataTypes::Vector4: {
				vec4_ = a.vector4();
				return (*this);
			}
			case DataTypes::Instruction: {
				instruction_ = a.instruction();
				return (*this);
			}
		}
		// Wtf?
		return (*this);
	}

	constexpr ValueData& operator=(const Color& color) {
		active_ = DataTypes::Color;
		rgba_ = color;
		return (*this);
	}

	constexpr ValueData& operator=(const float val) {
		active_ = DataTypes::Float;
		float_value_ = val;
		return (*this);
	}

	constexpr ValueData& operator=(const int val) {
		active_ = DataTypes::Int;
		integer_value_ = val;
		return (*this);
	}

	constexpr ValueData& operator=(const Vector2f& vec2) {
		active_ = DataTypes::Vector2;
		vec2_ = vec2;
		return (*this);
	}

	constexpr ValueData& operator=(const Vector3f& vec3) {
		active_ = DataTypes::Vector3;
		vec3_ = vec3;
		return (*this);
	}

	constexpr ValueData& operator=(const Vector4f& vec4) {
		active_ = DataTypes::Vector4;
		vec4_ = vec4;
		return (*this);
	}

	constexpr ValueData& operator=(const Instruction& instr) {
		active_ = DataTypes::Instruction;
		instruction_ = instr;
		return (*this);
	}

	// Checker functions
	constexpr bool is_none() const noexcept {
		return active_ == DataTypes::None;
	}

	constexpr bool is_rgba() const noexcept {
		return active_ == DataTypes::Color;
	}

	constexpr bool is_float() const noexcept {
		return active_ == DataTypes::Float;
	}

	constexpr bool is_int() const noexcept {
		return active_ == DataTypes::Int;
	}

	constexpr bool is_vector2() const noexcept {
		return active_ == DataTypes::Vector2;
	}

	constexpr bool is_vector3() const noexcept {
		return active_ == DataTypes::Vector3;
	}

	constexpr bool is_vector4() const noexcept {
		return active_ == DataTypes::Vector4;
	}

	constexpr bool is_instruction() const noexcept {
		return active_ == DataTypes::Instruction;
	}

	// Getters
	[[nodiscard]] constexpr auto rgba() const noexcept -> Color {
		return rgba_;
	}

	[[nodiscard]] constexpr auto float_value() const noexcept -> float {
		return float_value_;
	}

	[[nodiscard]] constexpr auto integer_value() const noexcept -> int {
		return integer_value_;
	}

	[[nodiscard]] constexpr auto vector2() const noexcept -> Vector2f {
		return vec2_;
	}

	[[nodiscard]] constexpr auto vector3() const noexcept -> Vector3f {
		return vec3_;
	}

	[[nodiscard]] constexpr auto vector4() const noexcept -> Vector4f {
		return vec4_;
	}

	[[nodiscard]] constexpr auto instruction() const noexcept -> Instruction {
		return instruction_;
	}

	[[nodiscard]] constexpr auto active() const noexcept -> DataTypes {
		return active_;
	}

private:
	DataTypes active_;

	union
	{
		Empty none_;
		Color rgba_;
		float float_value_;
		int integer_value_;
		Vector2f vec2_;
		Vector3f vec3_;
		Vector4f vec4_;
		Instruction instruction_;
	};
};

}	 // namespace cui::styles

#endif	  // CUI_STYLES_DATA_HPP
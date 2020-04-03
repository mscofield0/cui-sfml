#ifndef CUI_CT_VALUE_DATA_HPP
#define CUI_CT_VALUE_DATA_HPP

#include <compile_time/data_types/color.hpp>
#include <compile_time/data_types/vec.hpp>
#include <compile_time/data_types/instruction.hpp>
#include <compile_time/data_types/data_types.hpp>

namespace cui::ct {

using namespace cui::data_types;

struct Empty
{};

class ValueData
{
public:
	constexpr ValueData() : active_(DataTypes::None), none_() {}
	constexpr ValueData(const Color& color) : active_(DataTypes::Color), rgba_(color) {}
	constexpr ValueData(const float val) : active_(DataTypes::Float), float_value_(val) {}
	constexpr ValueData(const int val) : active_(DataTypes::Int), integer_value_(val) {}
	constexpr ValueData(const Vec2f& v2) : active_(DataTypes::Vec2), vec2_(v2) {}
	constexpr ValueData(const Vec3f& v3) : active_(DataTypes::Vec3), vec3_(v3) {}
	constexpr ValueData(const Vec4f& v4) : active_(DataTypes::Vec4), vec4_(v4) {}
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
			case DataTypes::Vec2: {
				vec2_ = a.vec2();
				return (*this);
			}
			case DataTypes::Vec3: {
				vec3_ = a.vec3();
				return (*this);
			}
			case DataTypes::Vec4: {
				vec4_ = a.vec4();
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

	constexpr ValueData& operator=(const Vec2f& p_vec2) {
		active_ = DataTypes::Vec2;
		vec2_ = p_vec2;
		return (*this);
	}

	constexpr ValueData& operator=(const Vec3f& p_vec3) {
		active_ = DataTypes::Vec3;
		vec3_ = p_vec3;
		return (*this);
	}

	constexpr ValueData& operator=(const Vec4f& p_vec4) {
		active_ = DataTypes::Vec4;
		vec4_ = p_vec4;
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

	constexpr bool is_vec2() const noexcept {
		return active_ == DataTypes::Vec2;
	}

	constexpr bool is_vec3() const noexcept {
		return active_ == DataTypes::Vec3;
	}

	constexpr bool is_vec4() const noexcept {
		return active_ == DataTypes::Vec4;
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

	[[nodiscard]] constexpr auto vec2() const noexcept -> Vec2f {
		return vec2_;
	}

	[[nodiscard]] constexpr auto vec3() const noexcept -> Vec3f {
		return vec3_;
	}

	[[nodiscard]] constexpr auto vec4() const noexcept -> Vec4f {
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
		Vec2f vec2_;
		Vec3f vec3_;
		Vec4f vec4_;
		Instruction instruction_;
	};
};

}	 // namespace cui::ct

#endif	  // #define CUI_CT_VALUE_DATA_HPP

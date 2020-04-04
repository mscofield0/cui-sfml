#ifndef CUI_VALUE_DATA_HPP
#define CUI_VALUE_DATA_HPP

#include <data_types/color.hpp>
#include <data_types/vec.hpp>
#include <data_types/instruction.hpp>
#include <data_types/data_types.hpp>
#include <compile_time/value_data.hpp>

namespace cui {

using namespace data_types;

struct Empty
{};

class ValueData
{
public:
	ValueData() : active_(DataTypes::None), none_(nullptr) {}
	ValueData(const Color& color) : active_(DataTypes::Color), rgba_(new Color(color)) {}
	ValueData(const float val) : active_(DataTypes::Float), float_value_(new float(val)) {}
	ValueData(const int val) : active_(DataTypes::Int), integer_value_(new int(val)) {}
	ValueData(const Vec2f& v2) : active_(DataTypes::Vec2), vec2_(new Vec2f(v2)) {}
	ValueData(const Vec3f& v3) : active_(DataTypes::Vec3), vec3_(new Vec3f(v3)) {}
	ValueData(const Vec4f& v4) : active_(DataTypes::Vec4), vec4_(new Vec4f(v4)) {}
	ValueData(const Instruction& instr) : active_(DataTypes::Instruction), instruction_(new Instruction(instr)) {}
	ValueData(const ValueData& other) {
		active_ = other.active_;
		switch (other.active_) {
			case DataTypes::None: {
				// Do nothing
				break;
			}
			case DataTypes::Color: {
				rgba_ = new Color(other.rgba());
				break;
			}
			case DataTypes::Float: {
				float_value_ = new float(other.float_value());
				break;
			}
			case DataTypes::Int: {
				integer_value_ = new int(other.integer_value());
				break;
			}
			case DataTypes::Vec2: {
				vec2_ = new Vec2f(other.vec2());
				break;
			}
			case DataTypes::Vec3: {
				vec3_ = new Vec3f(other.vec3());
				break;
			}
			case DataTypes::Vec4: {
				vec4_ = new Vec4f(other.vec4());
				break;
			}
			case DataTypes::Instruction: {
				instruction_ = new Instruction(other.instruction());
				break;
			}
		}
	}

	ValueData(const ct::ValueData& other) {
		active_ = other.active();
		switch (active_) {
			case DataTypes::None: {
				// Do nothing
				break;
			}
			case DataTypes::Color: {
				rgba_ = new Color(other.rgba());
				break;
			}
			case DataTypes::Float: {
				float_value_ = new float(other.float_value());
				break;
			}
			case DataTypes::Int: {
				integer_value_ = new int(other.integer_value());
				break;
			}
			case DataTypes::Vec2: {
				vec2_ = new Vec2f(other.vec2());
				break;
			}
			case DataTypes::Vec3: {
				vec3_ = new Vec3f(other.vec3());
				break;
			}
			case DataTypes::Vec4: {
				vec4_ = new Vec4f(other.vec4());
				break;
			}
			case DataTypes::Instruction: {
				instruction_ = new Instruction(other.instruction());
				break;
			}
		}
	}

	ValueData& operator=(const ValueData& a) {
		delete_current_active();
		active_ = a.active_;
		switch (a.active_) {
			case DataTypes::None: {
				return (*this);
			}
			case DataTypes::Color: {
				rgba_ = new Color(a.rgba());
				return (*this);
			}
			case DataTypes::Float: {
				float_value_ = new float(a.float_value());
				return (*this);
			}
			case DataTypes::Int: {
				integer_value_ = new int(a.integer_value());
				return (*this);
			}
			case DataTypes::Vec2: {
				vec2_ = new Vec2f(a.vec2());
				return (*this);
			}
			case DataTypes::Vec3: {
				vec3_ = new Vec3f(a.vec3());
				return (*this);
			}
			case DataTypes::Vec4: {
				vec4_ = new Vec4f(a.vec4());
				return (*this);
			}
			case DataTypes::Instruction: {
				instruction_ = new Instruction(a.instruction());
				return (*this);
			}
		}
		// Wtf?
		return (*this);
	}

	ValueData& operator=(const ct::ValueData& a) {
		delete_current_active();
		active_ = a.active();
		switch (active_) {
			case DataTypes::None: {
				return (*this);
			}
			case DataTypes::Color: {
				rgba_ = new Color(a.rgba());
				return (*this);
			}
			case DataTypes::Float: {
				float_value_ = new float(a.float_value());
				return (*this);
			}
			case DataTypes::Int: {
				integer_value_ = new int(a.integer_value());
				return (*this);
			}
			case DataTypes::Vec2: {
				vec2_ = new Vec2f(a.vec2());
				return (*this);
			}
			case DataTypes::Vec3: {
				vec3_ = new Vec3f(a.vec3());
				return (*this);
			}
			case DataTypes::Vec4: {
				vec4_ = new Vec4f(a.vec4());
				return (*this);
			}
			case DataTypes::Instruction: {
				instruction_ = new Instruction(a.instruction());
				return (*this);
			}
		}
		// Wtf?
		return (*this);
	}

	ValueData& operator=(Color* color) {
		active_ = DataTypes::Color;
		delete_current_active();
		rgba_ = color;
		return (*this);
	}

	ValueData& operator=(float* val) {
		active_ = DataTypes::Float;
		delete_current_active();
		float_value_ = val;
		return (*this);
	}

	ValueData& operator=(int* val) {
		active_ = DataTypes::Int;
		delete_current_active();
		integer_value_ = val;
		return (*this);
	}

	ValueData& operator=(Vec2f* p_vec2) {
		active_ = DataTypes::Vec2;
		delete_current_active();
		vec2_ = p_vec2;
		return (*this);
	}

	ValueData& operator=(Vec3f* p_vec3) {
		active_ = DataTypes::Vec3;
		delete_current_active();
		vec3_ = p_vec3;
		return (*this);
	}

	ValueData& operator=(Vec4f* p_vec4) {
		active_ = DataTypes::Vec4;
		delete_current_active();
		vec4_ = p_vec4;
		return (*this);
	}

	ValueData& operator=(Instruction* instr) {
		active_ = DataTypes::Instruction;
		delete_current_active();
		instruction_ = instr;
		return (*this);
	}

	ValueData& operator=(const Color& color) {
		rgba_ = new Color(color);
		return (*this);
	}

	ValueData& operator=(const float val) {
		float_value_ = new float(val);
		return (*this);
	}

	ValueData& operator=(const int val) {
		integer_value_ = new int(val);
		return (*this);
	}

	ValueData& operator=(const Vec2f& p_vec2) {
		vec2_ = new Vec2f(p_vec2);
		return (*this);
	}

	ValueData& operator=(const Vec3f& p_vec3) {
		vec3_ = new Vec3f(p_vec3);
		return (*this);
	}

	ValueData& operator=(const Vec4f& p_vec4) {
		vec4_ = new Vec4f(p_vec4);
		return (*this);
	}

	ValueData& operator=(const Instruction& instr) {
		instruction_ = new Instruction(instr);
		return (*this);
	}

	// Checker functions
	bool is_none() const noexcept {
		return active_ == DataTypes::None;
	}

	bool is_rgba() const noexcept {
		return active_ == DataTypes::Color;
	}

	bool is_float() const noexcept {
		return active_ == DataTypes::Float;
	}

	bool is_int() const noexcept {
		return active_ == DataTypes::Int;
	}

	bool is_vec2() const noexcept {
		return active_ == DataTypes::Vec2;
	}

	bool is_vec3() const noexcept {
		return active_ == DataTypes::Vec3;
	}

	bool is_vec4() const noexcept {
		return active_ == DataTypes::Vec4;
	}

	bool is_instruction() const noexcept {
		return active_ == DataTypes::Instruction;
	}

	// Getters
	[[nodiscard]] auto rgba() const noexcept -> Color& {
		return *rgba_;
	}

	[[nodiscard]] auto float_value() const noexcept -> float& {
		return *float_value_;
	}

	[[nodiscard]] auto integer_value() const noexcept -> int& {
		return *integer_value_;
	}

	[[nodiscard]] auto vec2() const noexcept -> Vec2f& {
		return *vec2_;
	}

	[[nodiscard]] auto vec3() const noexcept -> Vec3f& {
		return *vec3_;
	}

	[[nodiscard]] auto vec4() const noexcept -> Vec4f& {
		return *vec4_;
	}

	[[nodiscard]] auto instruction() const noexcept -> Instruction& {
		return *instruction_;
	}

	[[nodiscard]] auto active() const noexcept -> DataTypes {
		return active_;
	}

private:
	DataTypes active_;

	void delete_current_active() {
		switch (active_) {
			case DataTypes::None: {
				// Do nothing
				break;
			}
			case DataTypes::Color: {
				delete rgba_;
				break;
			}
			case DataTypes::Float: {
				delete float_value_;
				break;
			}
			case DataTypes::Int: {
				delete integer_value_;
				break;
			}
			case DataTypes::Vec2: {
				delete vec2_;
				break;
			}
			case DataTypes::Vec3: {
				delete vec3_;
				break;
			}
			case DataTypes::Vec4: {
				delete vec4_;
				break;
			}
			case DataTypes::Instruction: {
				delete instruction_;
				break;
			}
		}
	}

	union
	{
		Empty* none_;
		Color* rgba_;
		float* float_value_;
		int* integer_value_;
		Vec2f* vec2_;
		Vec3f* vec3_;
		Vec4f* vec4_;
		Instruction* instruction_;
	};
};

}	 // namespace cui

#endif	  // #define CUI_CT_VALUE_DATA_HPP

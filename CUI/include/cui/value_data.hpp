#ifndef CUI_VALUE_DATA_HPP
#define CUI_VALUE_DATA_HPP

#include <data_types/color.hpp>
#include <data_types/vec.hpp>
#include <data_types/instruction.hpp>
#include <data_types/data_types.hpp>
#include <compile_time/value_data.hpp>
#include <string>

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
	ValueData(const std::string& instr) : active_(DataTypes::String), string_(new std::string(instr)) {}
	ValueData(const ValueData& other) {
		active_ = other.active_;
		switch (other.active_) {
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
			case DataTypes::String: {
				string_ = new std::string(other.string());
				break;
			}
			default: {
				break;
			}
		}
	}

	ValueData(const ct::ValueData& other) {
		active_ = other.active();
		switch (active_) {
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
			case DataTypes::String: {
				string_ = new std::string(other.string().begin(), other.string().end());
				break;
			}
			default: {
				// Do nothing
				break;
			}
		}
	}

	ValueData& operator=(const ValueData& a) {
		delete_current_active();
		active_ = a.active_;
		switch (a.active_) {
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
			case DataTypes::String: {
				string_ = new std::string(a.string());
				return (*this);
			}
			default: {
				return (*this);
			}
		}
	}

	ValueData& operator=(const ct::ValueData& a) {
		delete_current_active();
		active_ = a.active();
		switch (active_) {
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
			case DataTypes::String: {
				string_ = new std::string(a.string().begin(), a.string().end());
				return (*this);
			}
			default: {
				return (*this);
			}
		}
	}

	ValueData& operator=(Color* color) {
		delete_current_active();
		active_ = DataTypes::Color;
		rgba_ = color;
		return (*this);
	}

	ValueData& operator=(float* val) {
		delete_current_active();
		active_ = DataTypes::Float;
		float_value_ = val;
		return (*this);
	}

	ValueData& operator=(int* val) {
		delete_current_active();
		active_ = DataTypes::Int;
		integer_value_ = val;
		return (*this);
	}

	ValueData& operator=(Vec2f* p_vec2) {
		delete_current_active();
		active_ = DataTypes::Vec2;
		vec2_ = p_vec2;
		return (*this);
	}

	ValueData& operator=(Vec3f* p_vec3) {
		delete_current_active();
		active_ = DataTypes::Vec3;
		vec3_ = p_vec3;
		return (*this);
	}

	ValueData& operator=(Vec4f* p_vec4) {
		delete_current_active();
		active_ = DataTypes::Vec4;
		vec4_ = p_vec4;
		return (*this);
	}

	ValueData& operator=(Instruction* instr) {
		delete_current_active();
		active_ = DataTypes::Instruction;
		instruction_ = instr;
		return (*this);
	}

	ValueData& operator=(std::string* str) {
		delete_current_active();
		active_ = DataTypes::String;
		string_ = str;
		return (*this);
	}

	ValueData& operator=(const Color& color) {
		delete_current_active();
		active_ = DataTypes::Color;
		rgba_ = new Color(color);
		return (*this);
	}

	ValueData& operator=(const float val) {
		delete_current_active();
		active_ = DataTypes::Float;
		float_value_ = new float(val);
		return (*this);
	}

	ValueData& operator=(const int val) {
		delete_current_active();
		active_ = DataTypes::Int;
		integer_value_ = new int(val);
		return (*this);
	}

	ValueData& operator=(const Vec2f& p_vec2) {
		delete_current_active();
		active_ = DataTypes::Vec2;
		vec2_ = new Vec2f(p_vec2);
		return (*this);
	}

	ValueData& operator=(const Vec3f& p_vec3) {
		delete_current_active();
		active_ = DataTypes::Vec3;
		vec3_ = new Vec3f(p_vec3);
		return (*this);
	}

	ValueData& operator=(const Vec4f& p_vec4) {
		delete_current_active();
		active_ = DataTypes::Vec4;
		vec4_ = new Vec4f(p_vec4);
		return (*this);
	}

	ValueData& operator=(const Instruction& instr) {
		delete_current_active();
		active_ = DataTypes::Instruction;
		instruction_ = new Instruction(instr);
		return (*this);
	}

	ValueData& operator=(const std::string& str) {
		delete_current_active();
		active_ = DataTypes::String;
		string_ = new std::string(str);
		return (*this);
	}

	// Checker functions
	[[nodiscard]] bool is_none() const noexcept {
		return active_ == DataTypes::None;
	}

	[[nodiscard]] bool is_rgba() const noexcept {
		return active_ == DataTypes::Color;
	}

	[[nodiscard]] bool is_float() const noexcept {
		return active_ == DataTypes::Float;
	}

	[[nodiscard]] bool is_int() const noexcept {
		return active_ == DataTypes::Int;
	}

	[[nodiscard]] bool is_vec2() const noexcept {
		return active_ == DataTypes::Vec2;
	}

	[[nodiscard]] bool is_vec3() const noexcept {
		return active_ == DataTypes::Vec3;
	}

	[[nodiscard]] bool is_vec4() const noexcept {
		return active_ == DataTypes::Vec4;
	}

	[[nodiscard]] bool is_instruction() const noexcept {
		return active_ == DataTypes::Instruction;
	}

	[[nodiscard]] bool is_string() const noexcept {
		return active_ == DataTypes::String;
	}

	// Getters
	[[nodiscard]] auto rgba() noexcept -> Color& {
		return *rgba_;
	}

	[[nodiscard]] auto rgba() const noexcept -> const Color& {
		return *rgba_;
	}

	[[nodiscard]] auto float_value() noexcept -> float& {
		return *float_value_;
	}

	[[nodiscard]] auto float_value() const noexcept -> float {
		return *float_value_;
	}

	[[nodiscard]] auto integer_value() noexcept -> int& {
		return *integer_value_;
	}

	[[nodiscard]] auto integer_value() const noexcept -> int {
		return *integer_value_;
	}

	[[nodiscard]] auto vec2() noexcept -> Vec2f& {
		return *vec2_;
	}

	[[nodiscard]] auto vec2() const noexcept -> const Vec2f& {
		return *vec2_;
	}

	[[nodiscard]] auto vec3() noexcept -> Vec3f& {
		return *vec3_;
	}

	[[nodiscard]] auto vec3() const noexcept -> const Vec3f& {
		return *vec3_;
	}

	[[nodiscard]] auto vec4() noexcept -> Vec4f& {
		return *vec4_;
	}

	[[nodiscard]] auto vec4() const noexcept -> const Vec4f& {
		return *vec4_;
	}

	[[nodiscard]] auto instruction() noexcept -> Instruction& {
		return *instruction_;
	}

	[[nodiscard]] auto instruction() const noexcept -> const Instruction& {
		return *instruction_;
	}

	[[nodiscard]] auto string() noexcept -> std::string& {
		return *string_;
	}

	[[nodiscard]] auto string() const noexcept -> const std::string& {
		return *string_;
	}

	[[nodiscard]] auto active() const noexcept -> DataTypes {
		return active_;
	}

private:
	void delete_current_active() {
		switch (active_) {
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
			case DataTypes::String: {
				delete string_;
				break;
			}
			default: {
				// Do nothing
				break;
			}
		}
	}

	DataTypes active_;

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
		std::string* string_;
	};
};

}	 // namespace cui

#endif	  // #define CUI_CT_VALUE_DATA_HPP

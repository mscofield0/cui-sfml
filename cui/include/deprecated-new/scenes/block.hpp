#ifndef CUI_SCENES_BLOCK_HPP
#define CUI_SCENES_BLOCK_HPP

#include <compile_time/string_view.hpp>
#include <compile_time/string_utils/trim_sv.hpp>
#include <scenes/detail/scene_parsing/valid_block_name.hpp>

// Enable constexpr Block
#ifdef CUI_ENABLE_CONSTEXPR_PARSING

#include <compile_time/static_vector.hpp>

namespace cui::scenes {

class Block
{
public:
	using style_vector = static_vector<StringView, 64>;

	constexpr Block() : name_(), style_list_{} {}
	constexpr Block(const StringView name) : name_(name), style_list_{} {}

	constexpr auto name() noexcept -> StringView& {
		return name_;
	}

	constexpr auto name() const noexcept -> StringView {
		return name_;
	}

	constexpr auto style_list() noexcept -> style_vector& {
		return style_list_;
	}

	constexpr auto style_list() const noexcept -> const style_vector& {
		return style_list_;
	}

	constexpr void add_style(StringView name) {
		style_list_.push_back(name);
	}

	constexpr auto parse_style_list(const StringView str) -> bool {
		if (str.empty()) return false;
		StringView::size_type prev = 0;
		StringView::size_type pos = str.find_first_of(',');
		while (pos != StringView::npos) {
			if (pos == prev) return false;
			auto extracted = str.substr(prev, pos - prev);
			auto t = trim_sv(extracted.substr(0, extracted.size() - 1));
			if (t.empty()) return false;
			if (!detail__::is_valid_block_name(t)) return false;
			add_style(t);
			prev = pos + 1;
			pos = str.find_first_of(',', prev);
		}
		if (prev == pos) return false;
		auto extracted = str.substr(prev, pos - prev);
		auto t = trim_sv(extracted.substr(0, extracted.size() - 1));
		if (t.empty()) return false;
		if (!detail__::is_valid_block_name(t)) return false;
		add_style(t);
		return true;
	}

private:
	StringView name_;
	style_vector style_list_;
};

}	 // namespace cui::scenes

#else	 // Disable constexpr block

#include <vector>

namespace cui::scenes {

class Block
{
public:
	using style_vector_type = std::vector<StringView>;

	Block() : name_(), style_list_{} {}
	Block(const StringView name) : name_(name), style_list_{} {}

	auto name() noexcept -> StringView& {
		return name_;
	}

	auto name() const noexcept -> StringView {
		return name_;
	}

	auto style_list() noexcept -> style_vector_type& {
		return style_list_;
	}

	auto style_list() const noexcept -> const style_vector_type& {
		return style_list_;
	}

	void add_style(const StringView name) {
		style_list_.push_back(name);
	}

	auto parse_style_list(const StringView& str) -> bool {
		if (str.empty()) return false;
		StringView::size_type prev = 0;
		StringView::size_type pos = str.find_first_of(',');
		while (pos != StringView::npos) {
			if (pos == prev) return false;
			auto extracted = str.substr(prev, pos - prev);
			auto t = trim_sv(extracted.substr(0, extracted.size() - 1));
			if (t.empty()) return false;
			if (!detail__::is_valid_block_name(t)) return false;
			add_style(t);
			prev = pos + 1;
			pos = str.find_first_of(',', prev);
		}
		if (prev == pos) return false;
		auto extracted = str.substr(prev, pos - prev);
		auto t = trim_sv(extracted.substr(0, extracted.size() - 1));
		if (t.empty()) return false;
		if (!detail__::is_valid_block_name(t)) return false;
		add_style(t);
		return true;
	}

private:
	StringView name_;
	style_vector_type style_list_;
};

}	 // namespace cui::scenes

#endif	  // constexpr Block

#endif	  // CUI_SCENES_BLOCK_HPP
#ifndef CUI_STYLES_DEFINITION_HPP
#define CUI_STYLES_DEFINITION_HPP

#include <global/whitespace.hpp>
#include <compile_time/string_view.hpp>
#include <compile_time/string_utils/trim_sv.hpp>
#include <styles/attribute.hpp>

// Enable constexpr Definition
#ifdef CUI_ENABLE_CONSTEXPR_PARSING

#include <styles/constants.hpp>
#include <compile_time/static_vector.hpp>

namespace cui::styles {

class Definition
{
public:
	using event_vector = cui::static_vector<cui::StringView, MAX_NUMBER_OF_EVENTS_IN_A_STYLE>;
	using attribute_vector = cui::static_vector<Attribute, MAX_NUMBER_OF_ATTRIBUTES_IN_A_STYLE>;

	constexpr Definition() : identifier_(), events_{}, attributes_{} {}
	constexpr Definition(const cui::StringView name) : identifier_(name), events_{}, attributes_{} {}

	[[nodiscard]] constexpr auto identifier() noexcept -> cui::StringView& {
		return identifier_;
	}

	[[nodiscard]] constexpr auto identifier() const noexcept -> cui::StringView {
		return identifier_;
	}

	[[nodiscard]] constexpr auto events() noexcept -> event_vector& {
		return events_;
	}

	[[nodiscard]] constexpr auto events() const noexcept -> const event_vector& {
		return events_;
	}

	[[nodiscard]] constexpr auto attributes() noexcept -> attribute_vector& {
		return attributes_;
	}

	[[nodiscard]] constexpr auto attributes() const noexcept -> const attribute_vector& {
		return attributes_;
	}

	constexpr void add_event(const cui::StringView event_name) {
		events_.push_back(event_name);
	}

	constexpr void add_attribute_name(const cui::StringView attr_name) {
		attributes_.emplace_back();
		attributes_.back().type() = attr_name;
	}

	constexpr void add_attribute_value(const cui::StringView attr_value) {
		attributes_.back().value() = attr_value;
	}

	[[nodiscard]] constexpr auto parse_events(const cui::StringView events) -> bool {
		if (events.empty()) return false;

		std::size_t prev = 0;
		std::size_t pos = events.find_first_of(',');
		while (pos != StringView::npos) {
			if (prev == pos) return false;
			auto temp_str = events.substr(prev, pos - prev);
			temp_str = trim_sv(temp_str);
			add_event(temp_str);
			prev = pos + 1;
			pos = events.find_first_of(',', prev);
		}
		if (prev == pos) return false;
		auto temp_str = events.substr(prev, pos - prev);
		temp_str = trim_sv(temp_str);
		add_event(temp_str);
		return true;
	}

private:
	cui::StringView identifier_;
	event_vector events_;
	attribute_vector attributes_;
};

}	 // namespace cui::styles

#else	 // Disable constexpr Definition

#include <vector>

namespace cui::styles {

class Definition
{
public:
	using event_vector = std::vector<cui::StringView>;
	using attribute_vector = std::vector<Attribute>;

	Definition() : identifier_(), events_{}, attributes_{} {}
	Definition(const cui::StringView name) : identifier_(name), events_{}, attributes_{} {}

	[[nodiscard]] auto identifier() noexcept -> cui::StringView& {
		return identifier_;
	}

	[[nodiscard]] auto identifier() const noexcept -> cui::StringView {
		return identifier_;
	}

	[[nodiscard]] auto events() noexcept -> event_vector& {
		return events_;
	}

	[[nodiscard]] auto events() const noexcept -> const event_vector& {
		return events_;
	}

	[[nodiscard]] auto attributes() noexcept -> attribute_vector& {
		return attributes_;
	}

	[[nodiscard]] auto attributes() const noexcept -> const attribute_vector& {
		return attributes_;
	}

	void add_event(const cui::StringView event_name) {
		events_.push_back(event_name);
	}

	void add_attribute_name(const cui::StringView attr_name) {
		attributes_.emplace_back();
		attributes_.back().type() = attr_name;
	}

	void add_attribute_value(const cui::StringView attr_value) {
		attributes_.back().value() = attr_value;
	}

	[[nodiscard]] auto parse_events(const cui::StringView events) -> bool {
		if (events.empty()) return false;

		std::size_t prev = 0;
		std::size_t pos = events.find_first_of(',');
		while (pos != StringView::npos) {
			if (prev == pos) return false;
			auto temp_str = events.substr(prev, pos - prev);
			temp_str = trim_sv(temp_str);
			add_event(temp_str);
			prev = pos + 1;
			pos = events.find_first_of(',', prev);
		}
		if (prev == pos) return false;
		auto temp_str = events.substr(prev, pos - prev);
		temp_str = trim_sv(temp_str);
		add_event(temp_str);
		return true;
	}

private:
	cui::StringView identifier_;
	event_vector events_;
	attribute_vector attributes_;
};

}	 // namespace cui::styles

#endif	  // constexpr Definition

#endif	  // CUI_STYLES_DEFINITION_HPP
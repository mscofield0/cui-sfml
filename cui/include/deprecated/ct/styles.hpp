#ifndef CUI_STYLES_HPP
#define CUI_STYLES_HPP

#include <array>
#include <iostream>
#include "../utility/utility.hpp"
#include "string.hpp"
#include "variant.hpp"
#include "static_string_stream.hpp"


namespace cui::ct {

using styles_value_t = cui::ct::string;

struct attribute
{
	cui::ct::string type;
	styles_value_t value;

	constexpr attribute() : type(""), value("") {}
	constexpr attribute(cui::ct::string type, styles_value_t value) : type(type), value(value) {}
};

struct styles_definition
{
	cui::ct::string styles_identifier;
	std::array<cui::ct::string, 128> events;
	std::array<attribute, 128> attributes;

	uint32_t event_idx;
	uint32_t attr_idx;

	constexpr styles_definition() : styles_identifier(""), events{}, attributes{}, event_idx(0), attr_idx(0) {}

	constexpr styles_definition(cui::ct::string name)
		: styles_identifier(name), events{}, attributes{}, event_idx(0), attr_idx(0) {}

	constexpr bool add_event(cui::ct::string name) {
		if (128 <= event_idx) {
			return false;
		}
		events[event_idx] = name;
		++event_idx;
		return true;
	}

	constexpr bool add_attribute_value(styles_value_t value) {
		if (128 <= attr_idx) {
			return false;
		}
		attributes[attr_idx].value = value;
		++attr_idx;
		return true;
	}

	constexpr bool add_attribute_name(cui::ct::string name) {
		if (128 <= attr_idx) {
			return false;
		}
		attributes[attr_idx].type = name;
		return true;
	}

	constexpr bool parse_events(cui::ct::string events_) noexcept {
		if (events_.size == 0) return false;
		uint32_t prev = 0;
		uint32_t pos = cui::ct::find(events_, ',', prev);
		while (pos != events_.size) {
			if (prev == pos) return false;
			add_event(cui::ct::trim(cui::ct::string{events_.str + prev, pos - prev}));
			prev = pos + 1;
			pos = cui::ct::find(events_, ',', prev);
		}
		if (prev == pos) return false;
		add_event(cui::ct::trim(cui::ct::string{events_.str + prev, events_.size - prev}));
		return true;
	}
};

template <const char* str>
constexpr uint32_t get_amount_of_definitions() {
	uint32_t current = 0;
	uint32_t amount = 0;
	while (str[current] != 0) {
		if (str[current] == '}') ++amount;
		++current;
	}
	return amount;
}

template <const char* str, uint32_t N>
constexpr cui::ct::variant<std::array<styles_definition, N>, error_report> parse_styles() {
	auto stream = cui::ct::static_string_stream{str};
	constexpr cui::ct::string delims{"[]{};:"};
	enum States : uint8_t
	{
		none = 0,
		onOpenSquareBracket,
		onCloseSquareBracket,
		onSemicolon,
		onOpenCurlyBracket,
		onCloseCurlyBracket,
		onColon
	} state = none;

	uint32_t idx = 0;
	constexpr auto amount_of_defs = get_amount_of_definitions<str>();
	std::array<styles_definition, amount_of_defs> defs;

	while (stream.good()) {
		auto consumed = cui::ct::consume(stream, delims);
		consumed.first = cui::ct::trim(consumed.first);
		switch (consumed.second) {
			case '[': {
				if (state != none) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: Expected a style definition closure '}'.", line, col};

					return err;
				}
				if (consumed.first.size == 0) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: Expected a style identifier.", line, col};

					return err;
				}

				// Pass on to token tree
				defs[idx].styles_identifier = consumed.first;
				state = onOpenSquareBracket;
				break;
			}
			case ']': {
				if (state != onOpenSquareBracket) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: Expected an event list definition '['.", line, col};

					return err;
				}
				if (consumed.first.size == 0) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: Expected an event list.", line, col};

					return err;
				}
				// Pass on to token tree
				if (!defs[idx].parse_events(consumed.first)) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: Incorrect event list.", line, col};

					return err;
				}

				state = onCloseSquareBracket;
				break;
			}
			case '{': {
				if (state != onCloseSquareBracket && state != none) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: Expected event list definition or beginning of a style definition.",
									 line,
									 col};

					return err;
				}
				if (state == none && consumed.first.size == 0) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: Expected a style identifier.", line, col};

					return err;
				}
				if (state == none) {
					// Pass on to token tree
					defs[idx].styles_identifier = consumed.first;
				}
				state = onOpenCurlyBracket;
				break;
			}
			case '}': {
				if (state != onOpenCurlyBracket && state != onSemicolon) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: Expected an style definition opening '{' or style attribute closure ';'.",
									 line,
									 col};

					return err;
				}
				++idx;
				state = none;
				break;
			}
			case ';': {
				if (state != onColon) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: Expected a colon.", line, col};

					return err;
				}
				if (consumed.first.size == 0) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: Expected an attribute value.", line, col};

					return err;
				}
				// Pass on to token tree
				if (!defs[idx].add_attribute_value(consumed.first)) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: Exceeded attribute quantity limit.", line, col};

					return err;
				}
				state = onSemicolon;
				break;
			}
			case ':': {
				if (state != onOpenCurlyBracket && state != onSemicolon) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: Expected an style definition opening '{' or style attribute closure ';'.",
									 line,
									 col};

					return err;
				}
				if (consumed.first.size == 0) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: Expected attribute name.", line, col};

					return err;
				}
				// Pass on to token tree
				if (!defs[idx].add_attribute_name(consumed.first)) {
					auto line = cui::ct::count_until_delim(cui::ct::string{str, stream.current + 1}, 0, '\n');
					auto last_idx = cui::ct::find_last(cui::ct::string{str, stream.current + 1}, '\n');
					auto col = stream.current - last_idx;
					error_report err{"ERROR: Beyond attribute quantity limit.", line, col};

					return err;
				}

				state = onColon;
				break;
			}
			default: {
				break;	  // "No delimiters found."
			}
		}
	}

	return defs;
}

}	 // namespace cui::ct

#endif	  //CUI_STYLES_HPP

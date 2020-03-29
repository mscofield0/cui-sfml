#ifndef CUI_STYLES_PARSER_HPP
#define CUI_STYLES_PARSER_HPP

#include <parse_utils/stream.hpp>
#include <parse_utils/stream_utils.hpp>
#include <global/constants.hpp>
#include <compile_time/string_view.hpp>
#include <compile_time/string_utils/trim_sv.hpp>
#include <compile_time/variant.hpp>
#include <styles/definition.hpp>
#include <styles/count_definitions.hpp>

// Enable constexpr Definition
#ifdef CUI_ENABLE_CONSTEXPR_PARSING

#include <compile_time/format.hpp>

namespace cui::styles {

namespace {

using namespace cui::ct;

template <std::size_t AOB>
constexpr auto return_value(const static_vector<styles::Definition, AOB>& result) {
	return result;
}

template <uint64_t StrSize, typename... Args>
constexpr auto return_value(const char (&s)[StrSize], Args&&... args) {
	return create_formatted_string_with_max_size<512>(s, std::move(args)...);
}

}	 // namespace

template <const char* str>
constexpr auto parse()
  -> cui::Variant<static_vector<styles::Definition, styles::count_definitions(str)>, ct::Format<512>> {
	StringView data{str};
	CharStream stream(data);
	constexpr StringView delims("[]{};:");
	constexpr auto AOD = count_definitions(str);

	constexpr const char missing_styles_identifier[] = "Expected a styles identifier";
	constexpr const char expected_end_of_style[] = "Expected end of style";
	constexpr const char invalid_event_list[] = "Invalid event list";
	constexpr const char expected_identifier_or_event_list[] = "Expected a styles identifier or an event list";
	constexpr const char expected_end_of_style_or_attribute[] =
	  "Expected an open curly brace or the end of an attribute";
	constexpr const char expected_attribute_identifier_and_colon[] = "Expected an attribute identifier and a colon";
	constexpr const char empty_attribute_value[] = "Attribute value is empty";
	constexpr const char expected_event_list_definition_symbol[] = "Expected a '[' previously";
	constexpr const char missing_event_list[] = "Expected an event list";

	enum class States : uint8_t
	{
		None = 0,
		OnOpenSquareBracket = '[',
		OnCloseSquareBracket = ']',
		OnSemicolon = ';',
		OnOpenCurlyBracket = '{',
		OnCloseCurlyBracket = '}',
		OnColon = ':'
	} state = States::None;

	static_vector<Definition, AOD> defs;

	while (stream.good()) {
		auto consumed = consume(stream, delims);
		auto trimmed = trim_sv(consumed.first);

		const auto temp_state = static_cast<States>(consumed.second);
		switch (temp_state) {
			case States::OnOpenSquareBracket: {
				if (state != States::None) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										expected_end_of_style);
				}
				if (trimmed.empty()) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										missing_styles_identifier);
				}

				defs.emplace_back();
				defs.back().identifier() = trimmed;
				state = States::OnOpenSquareBracket;
				break;
			}
			case States::OnCloseSquareBracket: {
				if (state != States::OnOpenSquareBracket) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										expected_event_list_definition_symbol);
				}

				if (trimmed.empty()) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										missing_event_list);
				}

				if (!defs.back().parse_events(trimmed)) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										invalid_event_list);
				}

				state = States::OnCloseSquareBracket;
				break;
			}
			case States::OnOpenCurlyBracket: {
				if (state != States::OnCloseSquareBracket && state != States::None) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										expected_identifier_or_event_list);
				}

				if (state == States::None && trimmed.empty()) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										missing_styles_identifier);
				}

				if (state == States::None) {
					defs.emplace_back();
					defs.back().identifier() = trimmed;
				}

				state = States::OnOpenCurlyBracket;
				break;
			}

			case States::OnCloseCurlyBracket: {
				if (state != States::OnOpenCurlyBracket && state != States::OnSemicolon) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										expected_end_of_style_or_attribute);
				}

				state = States::None;
				break;
			}

			case States::OnSemicolon: {
				if (state != States::OnColon) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										expected_attribute_identifier_and_colon);
				}

				if (trimmed.empty()) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										empty_attribute_value);
				}

				defs.back().add_attribute_value(trimmed);

				state = States::OnSemicolon;
				break;
			}

			case States::OnColon: {
				if (state != States::OnOpenCurlyBracket && state != States::OnSemicolon) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										expected_end_of_style_or_attribute);
				}

				if (trimmed.empty()) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										empty_attribute_value);
				}

				defs.back().add_attribute_name(trimmed);
				state = States::OnColon;
				break;
			}

			default: {
				break;	  // "No delimiters found."
			}
		}
	}

	return defs;
}

}	 // namespace cui::styles

#else	 // Disable constexpr styles parsing

#include <global/format.hpp>

#include <vector>

namespace cui::styles {

namespace {

auto return_value(const std::vector<styles::Definition>& result) {
	return result;
}

template <uint64_t StrSize, typename... Args>
auto return_value(const char (&s)[StrSize], Args&&... args) {
	return create_formatted_string(s, std::move(args)...);
}

}	 // namespace

auto parse(const StringView data) -> cui::Variant<std::vector<cui::styles::Definition>, Format> {
	CharStream stream(data);
	constexpr StringView delims("[]{};:");

	constexpr const char missing_styles_identifier[] = "Expected a styles identifier";
	constexpr const char expected_end_of_style[] = "Expected end of style";
	constexpr const char invalid_event_list[] = "Invalid event list";
	constexpr const char expected_identifier_or_event_list[] = "Expected a styles identifier or an event list";
	constexpr const char expected_end_of_style_or_attribute[] =
	  "Expected an open curly brace or the end of an attribute";
	constexpr const char expected_attribute_identifier_and_colon[] = "Expected an attribute identifier and a colon";
	constexpr const char empty_attribute_value[] = "Attribute value is empty";
	constexpr const char expected_event_list_definition_symbol[] = "Expected a '[' previously";
	constexpr const char missing_event_list[] = "Expected an event list";

	enum class States : uint8_t
	{
		None = 0,
		OnOpenSquareBracket = '[',
		OnCloseSquareBracket = ']',
		OnSemicolon = ';',
		OnOpenCurlyBracket = '{',
		OnCloseCurlyBracket = '}',
		OnColon = ':'
	} state = States::None;

	std::vector<Definition> defs;
	defs.reserve(count_definitions(data));

	while (stream.good()) {
		auto consumed = consume(stream, delims);
		auto trimmed = trim_sv(consumed.first);

		const auto temp_state = static_cast<States>(consumed.second);
		switch (temp_state) {
			case States::OnOpenSquareBracket: {
				if (state != States::None) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										expected_end_of_style);
				}
				if (trimmed.empty()) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										missing_styles_identifier);
				}

				defs.emplace_back();
				defs.back().identifier() = trimmed;
				state = States::OnOpenSquareBracket;
				break;
			}
			case States::OnCloseSquareBracket: {
				if (state != States::OnOpenSquareBracket) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										expected_event_list_definition_symbol);
				}

				if (trimmed.empty()) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										missing_event_list);
				}

				if (!defs.back().parse_events(trimmed)) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										invalid_event_list);
				}

				state = States::OnCloseSquareBracket;
				break;
			}
			case States::OnOpenCurlyBracket: {
				if (state != States::OnCloseSquareBracket && state != States::None) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										expected_identifier_or_event_list);
				}

				if (state == States::None && trimmed.empty()) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										missing_styles_identifier);
				}

				if (state == States::None) {
					defs.emplace_back();
					defs.back().identifier() = trimmed;
				}

				state = States::OnOpenCurlyBracket;
				break;
			}

			case States::OnCloseCurlyBracket: {
				if (state != States::OnOpenCurlyBracket && state != States::OnSemicolon) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										expected_end_of_style_or_attribute);
				}

				state = States::None;
				break;
			}

			case States::OnSemicolon: {
				if (state != States::OnColon) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										expected_attribute_identifier_and_colon);
				}

				if (trimmed.empty()) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										empty_attribute_value);
				}

				defs.back().add_attribute_value(trimmed);

				state = States::OnSemicolon;
				break;
			}

			case States::OnColon: {
				if (state != States::OnOpenCurlyBracket && state != States::OnSemicolon) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										expected_end_of_style_or_attribute);
				}

				if (trimmed.empty()) {
					return return_value("ERROR at LINE:{}, COL:{} :: {}",
										uint64_t{stream.line()},
										uint64_t{stream.line()},
										empty_attribute_value);
				}

				defs.back().add_attribute_name(trimmed);
				state = States::OnColon;
				break;
			}

			default: {
				break;	  // "No delimiters found."
			}
		}
	}

	return return_value(defs);
}

}	 // namespace cui::styles

#endif

#endif	  // CUI_STYLES_PARSER_HPP
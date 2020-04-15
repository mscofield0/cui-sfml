#ifndef CUI_SFML_EVENT_DATA_HPP
#define CUI_SFML_EVENT_DATA_HPP

#include <string_view>
#include <variant>

#include <SFML/Window/Event.hpp>
#include <cui/visual/node.hpp>

namespace cui {

template <typename TNode>
class EventData
{
	struct Empty
	{};

public:
	using node_t = TNode;
	using size_type = std::size_t;
	using data_variant_t = std::variant<Empty,
										sf::Event::SizeEvent,
										sf::Event::KeyEvent,
										sf::Event::TextEvent,
										sf::Event::MouseMoveEvent,
										sf::Event::MouseButtonEvent,
										sf::Event::MouseWheelEvent,
										sf::Event::MouseWheelScrollEvent,
										sf::Event::JoystickConnectEvent,
										sf::Event::JoystickMoveEvent,
										sf::Event::JoystickButtonEvent,
										sf::Event::TouchEvent,
										sf::Event::SensorEvent>;

	EventData() : data_(Empty{}), caller_(nullptr), caller_index_(-1) {}
	template <typename TData>
	EventData(const TData& p_data, std::string_view p_name)
		: data_(p_data), caller_(nullptr), caller_index_(-1), event_name_(p_name) {}
	template <typename TData>
	EventData(const TData& p_data, node_t* p_caller, size_type p_index, std::string_view p_name)
		: data_(p_data), caller_(p_caller), caller_index_(p_index), event_name_(p_name) {}

	[[nodiscard]] auto get() noexcept -> data_variant_t& {
		return data_;
	}

	[[nodiscard]] auto get() const noexcept -> const data_variant_t& {
		return data_;
	}

	[[nodiscard]] auto caller() noexcept -> node_t* {
		return caller_;
	}

	[[nodiscard]] auto caller() const noexcept -> const node_t* {
		return caller_;
	}

	[[nodiscard]] auto caller_index() noexcept -> size_type& {
		return caller_index_;
	}

	[[nodiscard]] auto caller_index() const noexcept -> size_type {
		return caller_index_;
	}

	[[nodiscard]] auto event_name() const noexcept -> std::string_view {
		return event_name_;
	}

	[[nodiscard]] bool has_caller() const noexcept {
		return caller_ != nullptr;
	}

	[[nodiscard]] bool empty() const noexcept {
		return data_.index() == 0;
	}

private:
	data_variant_t data_;
	node_t* caller_;
	size_type caller_index_;
	std::string_view event_name_;
};

}	 // namespace cui

#endif	  // CUI_SFML_EVENT_DATA_HPP
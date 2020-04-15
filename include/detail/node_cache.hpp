#ifndef CUI_SFML_NODE_CACHE_HPP
#define CUI_SFML_NODE_CACHE_HPP

#include <functional>

#include <SFML/Window/Event.hpp>

#include <detail/event_data.hpp>

namespace cui {

class NodeCache
{
public:
	void reset() noexcept {
		focused_ = false;
		hovered_ = false;
	}

	[[nodiscard]] auto focused() noexcept -> bool& {
		return focused_;
	}

	[[nodiscard]] auto focused() const noexcept -> const bool& {
		return focused_;
	}

	[[nodiscard]] auto hovered() noexcept -> bool& {
		return hovered_;
	}

	[[nodiscard]] auto hovered() const noexcept -> const bool& {
		return hovered_;
	}

	[[nodiscard]] auto text() noexcept -> std::string& {
		return text_;
	}

	[[nodiscard]] auto text() const noexcept -> const std::string& {
		return text_;
	}

private:
	bool focused_;
	bool hovered_;
	std::string text_;
};

}	 // namespace cui

#endif	  // CUI_SFML_NODE_CACHE_HPP
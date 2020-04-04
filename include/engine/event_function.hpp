#ifndef CUI_SFML_EVENT_FUNCTION_HPP
#define CUI_SFML_EVENT_FUNCTION_HPP

#include <functional>

namespace cui {

class EventFunction
{
public:
	template <typename Func>
	EventFunction(bool p_will_modify, Func&& p_func) : will_modify_(p_will_modify), func_(std::move(p_func)) {}

	void operator()() const {
		func_();
	}

	void execute() const {
		func_();
	}

	[[nodiscard]] bool will_modify() const noexcept {
		return will_modify_;
	}

private:
	const bool will_modify_;
	const std::function<void()> func_;
};

}	 // namespace cui

#endif	  // CUI_SFML_EVENT_FUNCTION_HPP
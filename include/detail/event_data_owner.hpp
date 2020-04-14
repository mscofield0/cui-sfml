#ifndef CUI_SFML_EVENT_DATA_OWNER_HPP
#define CUI_SFML_EVENT_DATA_OWNER_HPP

#include <detail/event_cache.hpp>
#include <detail/event_data.hpp>

namespace cui {

class EventDataOwner {
public:
	EventDataOwner(EventDataStack& event_stack) : stack_(event_stack) {}

	template <typename TData>
	void create_data(const TData& data) {
		inserted_ = stack_.emplace(stack_.end(), data);
	}

	template <typename TData>
	void create_data(TData&& data) {
		inserted_ = stack_.emplace(stack_.end(), std::move(data));
	}

	~EventDataOwner() {
		stack_.erase(inserted_);
	}
private:
	EventDataStack& stack_;
	EventDataStack::const_iterator inserted_;
};

}

#endif // CUI_SFML_EVENT_DATA_OWNER_HPP
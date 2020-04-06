#ifndef CUI_EVENT_PREDICATE_HPP
#define CUI_EVENT_PREDICATE_HPP

#include <functional>

namespace cui {

using event_predicate = std::function<void()>;

}	 // namespace cui

#endif	  // CUI_EVENT_PREDICATE_HPP


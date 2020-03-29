#ifndef CUI_GLOBAL_REASSIGN_TYPE_HPP
#define CUI_GLOBAL_REASSIGN_TYPE_HPP

namespace cui {

template <typename T, typename... Args>
constexpr void reassign_type(T& ref, const Args&... args) {
	ref = T{args...};
}

}	 // namespace cui

#endif	  // CUI_GLOBAL_REASSIGN_TYPE_HPP
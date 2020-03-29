#ifndef CUI_TRACKED_ARRAY_HPP
#define CUI_TRACKED_ARRAY_HPP

#include <global/array.hpp>

namespace cui {

template <typename T, std::size_t Size>
class TrackedArray : public Array<T, Size> {
public:
    using base_t = Array<T, Size>;
	using value_type = typename base_t::value_type;
	using size_type = typename base_t::size_type;
	using difference_type = typename base_t::difference_type;
	using reference = typename base_t::reference;
	using const_reference = typename base_t::const_reference;
	using pointer = typename base_t::pointer;
	using const_pointer = typename base_t::const_pointer;
	using iterator = typename base_t::iterator;
	using const_iterator = typename base_t::const_iterator;

    constexpr TrackedArray() : base_t{0}, tracker_{0} {}
    constexpr TrackedArray(const_reference val) : base_t{val}, tracker_{0} {}
    constexpr TrackedArray(size_type tracker, const_reference val) : base_t{val}, tracker_{tracker} {}
 
    template <typename... Args>
    constexpr TrackedArray(Args... args) : base_t{args...}, tracker_{0} {}
    template <typename... Args>
    constexpr TrackedArray(size_type tracker, Args... args) : base_t{args...}, tracker_{tracker} {}

    constexpr reference current_item() noexcept {
        return *(tracker_);
    }
    
    constexpr const_reference current_item() const noexcept {
        return *(tracker_);
    }

    constexpr void change_tracked_item(size_type idx) noexcept {
        if(idx >= this->size()) throw ExceptionCT{"Out of range"};
        tracker_ = this->begin() + idx;
    } 
    
    constexpr void change_tracked_item(iterator it) noexcept {
        if(it >= this->end()) throw ExceptionCT{"Out of range"};
        tracker_ = it;
    }
protected:
    iterator tracker_;
};

}

#endif	  // CUI_TRACKED_ARRAY_HPP
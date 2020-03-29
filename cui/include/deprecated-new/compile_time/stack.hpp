#ifndef CUI_COMPILE_TIME_STACK_HPP
#define CUI_COMPILE_TIME_STACK_HPP

#include <global/constants.hpp>
#include <compile_time/static_vector.hpp>
#include <compile_time/exception.hpp>

namespace cui {

template <typename T, std::size_t MaxSize>
class Stack {
public:
    using stack_vector_type = static_vector<T, MaxSize>;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;

    constexpr Stack() : data_{} {}

    constexpr std::size_t size() const noexcept {
        return data_.size();
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return size() == 0;
    } 

    constexpr void push(const_reference item) {
        data_.push_back(item);
    }

    constexpr const_reference top() const noexcept {
        return data_.back();
    }
    
    constexpr reference top() noexcept {
        return data_.back();
    }

    constexpr T pop() {
        const auto& t = data_.back();
        data_.pop_back();
        return t;
    }

    constexpr void remove_top(std::size_t n) {
        if(n >= size()) throw ExceptionCT{"Cannot remove more elements than the size dictates"};
        data_.erase(data_.end() - n - 1);
    }

    constexpr void clear() noexcept {
        data_.clear();
    }
    
private:
    stack_vector_type data_;
};

}


#endif // CUI_COMPILE_TIME_STACK_HPP
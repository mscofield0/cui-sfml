#ifndef CUI_PARSE_UTILS_STREAM_HPP
#define CUI_PARSE_UTILS_STREAM_HPP

#include <iterator>

#include <compile_time/string_utils/char_traits.hpp>
#include <compile_time/string_view.hpp>

namespace cui {

/// \class Stream
/// \brief Used to provide stream-like functionality to \a cui::BasicStringView
template <typename CharT, typename Traits = BasicCharTraits<CharT>>
class Stream
{
private:
	/// \brief Sets the \see Stream object to a bad state
	/// \details The bad state prevents the \see Stream from advancing
	constexpr void set_bad_stream() noexcept {
		state_ = false;
	}

public:
	using string_view_type = BasicStringView<CharT, Traits>;
	using sv_iterator = typename string_view_type::iterator;
	using sv_const_iterator = typename string_view_type::const_iterator;

	/// \brief Advances the \see Stream object until it hits a newline character
	/// \param data A \a cui::BasicStringView<CharT,Traits> to enable stream-like functionality on
	constexpr Stream(const string_view_type data)
		: str_(data), begin_(str_.begin()), end_(str_.end()), current_(begin_), state_(true), line_(1), col_(0) {
		state_ = begin_ != end_;
	}

	/// \brief Gets the internal \see str_ begin iterator
	/// \return Returns a \a cui::BasicStringView<CharT,Traits>::iterator
	[[nodiscard]] constexpr sv_iterator begin() noexcept {
		return begin_;
	}

	/// \brief Gets the internal \see str_ begin iterator
	/// \return Returns a \a cui::BasicStringView<CharT,Traits>::iterator marked with const
	[[nodiscard]] constexpr sv_iterator begin() const noexcept {
		return begin_;
	}

	/// \brief Gets the internal \see str_ end iterator
	/// \return Returns a \a cui::BasicStringView<CharT,Traits>::iterator
	[[nodiscard]] constexpr sv_iterator end() noexcept {
		return end_;
	}

	/// \brief Gets the internal \see str_ end iterator
	/// \return Returns a \a cui::BasicStringView<CharT,Traits>::iterator marked with const
	[[nodiscard]] constexpr sv_iterator end() const noexcept {
		return end_;
	}

	/// \brief Gets the internal \see current_ iterator that marks
	/// 		the current position in the \see Stream
	/// \return Returns a \a cui::BasicStringView<CharT,Traits>::iterator
	[[nodiscard]] constexpr sv_iterator current() noexcept {
		return current_;
	}

	/// \brief Gets the internal \see current_ iterator that marks
	/// 		the current position in the \see Stream
	/// \return Returns a \a cui::BasicStringView<CharT,Traits>::iterator marked with const
	[[nodiscard]] constexpr sv_iterator current() const noexcept {
		return current_;
	}

	/// \brief Gets the internal \see str_ variable
	/// \return Returns a \a cui::BasicStringView<CharT,Traits>
	[[nodiscard]] constexpr StringView data() const noexcept {
		return str_;
	}

	/// \brief Looks at the next iterator in the \see Stream
	/// \details If the \see Stream object is invalid (i.e. the next
	/// 		 iterator is the end iterator) then the function
	/// 		 returns the end iterator
	/// \return Returns the following iterator in the \see Stream object
	[[nodiscard]] constexpr sv_iterator peek() noexcept {
		if (current_ + 1 == end_) {
			set_bad_stream();
			return end_;
		}
		return current_ + 1;
	}

	/// \brief Returns the distance between the begin and current iterator
	/// \return Returns an according \a difference_type
	[[nodiscard]] constexpr typename std::iterator_traits<sv_iterator>::difference_type distance() const noexcept {
		return std::distance(begin(), current());
	}

	/// \brief Advances and returns the following iterator
	/// \details If the \see Stream object is invalid (i.e. the state is bad)
	/// 		 then the function returns the end iterator
	/// \return Returns the following iterator in the \see Stream object
	constexpr sv_iterator next() noexcept {
		if (peek() == end_) {
			return end_;
		}
		++current_;
		++col_;
		if (*current_ == '\n') {
			++line_;
			col_ = 0;
		}
		return current_;
	}

	/// \brief Returns whether the \see Stream object is valid
	/// \details If the \see Stream object is good, it returns \a true
	/// 		 If the \see Stream object is bad, it returns false
	/// \return Returns the internal \see state_ variable
	[[nodiscard]] constexpr bool good() const noexcept {
		return state_;
	}

	/// \brief Returns the current line number
	/// \return Returns the current line number
	[[nodiscard]] constexpr std::size_t line() const noexcept {
		return line_;
	}

	/// \brief Returns the current column number
	/// \return Returns the current column number
	[[nodiscard]] constexpr std::size_t col() const noexcept {
		return col_;
	}

private:
	string_view_type str_;
	sv_iterator begin_;
	sv_iterator end_;
	sv_iterator current_;

	bool state_;
	std::size_t line_;
	std::size_t col_;
};

using CharStream = Stream<char>;

}	 // namespace cui

#endif	  // CUI_PARSE_UTILS_STREAM_HPP
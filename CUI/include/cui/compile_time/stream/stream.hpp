#ifndef CUI_CT_STREAM_HPP
#define CUI_CT_STREAM_HPP

#include <iterator>

#include <compile_time/string/char_traits.hpp>
#include <compile_time/string/string_view.hpp>
#include <aliases.hpp>

namespace cui::ct {

/// \class Stream
/// \brief Used to provide stream-like functionality to \a cui::BasicStringView
template <typename CharT, typename Traits = BasicCharTraits<CharT>>
class Stream
{
public:
	using string_view_t = BasicStringView<CharT, Traits>;
	using iterator = typename string_view_t::iterator;
	using const_iterator = typename string_view_t::const_iterator;

	/// \param data A \a cui::BasicStringView<CharT,Traits> to enable stream-like functionality on
	constexpr Stream(string_view_t data)
		: str_(data), begin_(str_.begin()), end_(str_.end()), current_(begin_), state_(true), line_(1), col_(0) {
		state_ = begin_ != end_;
	}

	/// \brief Gets the internal \see str_ begin iterator
	/// \return Returns a \a cui::BasicStringView<CharT,Traits>::iterator marked with const
	[[nodiscard]] constexpr auto begin() const noexcept -> const_iterator {
		return begin_;
	}

	/// \brief Gets the internal \see str_ end iterator
	/// \return Returns a \a cui::BasicStringView<CharT,Traits>::iterator marked with const
	[[nodiscard]] constexpr auto end() const noexcept -> const_iterator {
		return end_;
	}

	/// \brief Gets the internal \see current_ iterator that marks
	/// 		the current position in the \see Stream
	/// \return Returns a \a cui::BasicStringView<CharT,Traits>::iterator marked with const
	[[nodiscard]] constexpr auto current() const noexcept -> const_iterator {
		return current_;
	}

	/// \brief Gets the internal \see str_ variable
	/// \return Returns a \a cui::BasicStringView<CharT,Traits>
	[[nodiscard]] constexpr auto data() const noexcept -> const string_view_t {
		return str_;
	}

	/// \brief Looks at the next iterator in the \see Stream
	/// \details If the \see Stream object is invalid (i.e. the next
	/// 		 iterator is the end iterator) then the function
	/// 		 returns the end iterator
	/// \return Returns the following iterator in the \see Stream object
	[[nodiscard]] constexpr auto peek() noexcept -> iterator {
		if (current_ + 1 == end_) {
			set_bad_stream();
			return end_;
		}
		return current_ + 1;
	}

	/// \brief Returns the distance between the begin and current iterator
	/// \return Returns an according \a difference_type
	[[nodiscard]] constexpr auto distance() const noexcept -> int {
		return current() - begin();
	}

	/// \brief Advances and returns the following iterator
	/// \details If the \see Stream object is invalid (i.e. the state is bad)
	/// 		 then the function returns the end iterator
	/// \return Returns the following iterator in the \see Stream object
	constexpr auto next() noexcept -> iterator {
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
	[[nodiscard]] constexpr auto line() const noexcept -> u64 {
		return line_;
	}

	/// \brief Returns the current column number
	/// \return Returns the current column number
	[[nodiscard]] constexpr auto col() const noexcept -> u64 {
		return col_;
	}

private:
	/// \brief Sets the \see Stream object to a bad state
	/// \details The bad state prevents the \see Stream from advancing
	constexpr void set_bad_stream() noexcept {
		state_ = false;
	}

	string_view_t str_;
	iterator begin_;
	iterator end_;
	iterator current_;

	bool state_;
	u64 line_;
	u64 col_;
};

using CharStream = Stream<char>;

}	 // namespace cui::ct

#endif	  // CUI_CT_STREAM_HPP
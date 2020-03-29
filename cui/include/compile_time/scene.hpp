#ifndef CUI_CT_SCENE_HPP
#define CUI_CT_SCENE_HPP

#include <compile_time/string/string_view.hpp>
#include <compile_time/scenes/block.hpp>
#include <containers/static_vector.hpp>
#include <aliases.hpp>

namespace cui::ct {

template <u64 BlockAmount>
class Scene
{
public:
	using blocks_t = StaticVector<scenes::Block, BlockAmount>;
	using children_t = StaticVector<StaticVector<u64, BlockAmount>, BlockAmount>;
	using depths_t = StaticVector<u64, BlockAmount>;

	constexpr Scene() : blocks_{}, children_{}, depths_{} {}

	[[nodiscard]] constexpr auto blocks() noexcept -> blocks_t& {
		return blocks_;
	}

	[[nodiscard]] constexpr auto blocks() const noexcept -> const blocks_t& {
		return blocks_;
	}

	[[nodiscard]] constexpr auto children() noexcept -> children_t& {
		return children_;
	}

	[[nodiscard]] constexpr auto children() const noexcept -> const children_t& {
		return children_;
	}

	[[nodiscard]] constexpr auto depths() noexcept -> depths_t& {
		return depths_;
	}

	[[nodiscard]] constexpr auto depths() const noexcept -> const depths_t& {
		return depths_;
	}

	[[nodiscard]] constexpr auto index_of_last() const noexcept -> u64 {
		return blocks().size() - 1;
	}

	constexpr void add_child_to(u64 to, u64 child_idx) {
		children_[to].push_back(child_idx);
	}

private:
	blocks_t blocks_;
	children_t children_;
	depths_t depths_;
};

}	 // namespace cui::ct

#endif	  // CUI_CT_SCENE_HPP
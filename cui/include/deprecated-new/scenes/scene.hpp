#ifndef CUI_SCENES_SCENE_HPP
#define CUI_SCENES_SCENE_HPP

#include <compile_time/string_view.hpp>
#include <compile_time/string_utils/trim_sv.hpp>
#include <scenes/block.hpp>
#include <global/constants.hpp>

// Enable constexpr Scene
#ifdef CUI_ENABLE_CONSTEXPR_PARSING

#include <compile_time/static_vector.hpp>

namespace cui::scenes {

template <std::size_t BlockAmount>
class Scene
{
public:
	static constexpr std::size_t block_amount = BlockAmount;
	using blocks_vector_type = static_vector<Block, BlockAmount>;
	using children_vector_type = static_vector<static_vector<int, BlockAmount>, BlockAmount>;
	using depths_vector_type = static_vector<std::size_t, BlockAmount>;

	constexpr Scene() : blocks_{}, children_{}, depths_{} {}

	[[nodiscard]] constexpr auto blocks() noexcept -> blocks_vector_type& {
		return blocks_;
	}

	[[nodiscard]] constexpr auto blocks() const noexcept -> const blocks_vector_type& {
		return blocks_;
	}

	[[nodiscard]] constexpr auto children() noexcept -> children_vector_type& {
		return children_;
	}

	[[nodiscard]] constexpr auto children() const noexcept -> const children_vector_type& {
		return children_;
	}

	[[nodiscard]] constexpr auto depths() noexcept -> depths_vector_type& {
		return depths_;
	}

	[[nodiscard]] constexpr auto depths() const noexcept -> const depths_vector_type& {
		return depths_;
	}

	[[nodiscard]] constexpr auto index_of_last() const noexcept -> std::size_t {
		return blocks().size() - 1;
	}

	constexpr void add_child_to(std::size_t to, std::size_t child_idx) {
		children_[to].push_back(child_idx);
	}

private:
	blocks_vector_type blocks_;
	children_vector_type children_;
	depths_vector_type depths_;
};

}	 // namespace cui::scenes

#else	 // Disable constexpr Scene

#include <vector>

namespace cui::scenes {

class Scene
{
public:
	using blocks_vector_type = std::vector<Block>;
	using children_vector_type = std::vector<std::vector<int>>;
	using depths_vector_type = std::vector<std::size_t>;

	Scene(std::size_t aob) : blocks_{}, children_{}, depths_{} {
		blocks_.reserve(aob);
		children_.reserve(aob);
		depths_.reserve(aob);
		for (std::size_t i = 0; i < aob; ++i) {
			blocks_.emplace_back();
			children_.emplace_back();
			depths_.emplace_back();
		}
	}

	[[nodiscard]] constexpr auto blocks() noexcept -> blocks_vector_type& {
		return blocks_;
	}

	[[nodiscard]] constexpr auto blocks() const noexcept -> const blocks_vector_type& {
		return blocks_;
	}

	[[nodiscard]] constexpr auto children() noexcept -> children_vector_type& {
		return children_;
	}

	[[nodiscard]] constexpr auto children() const noexcept -> const children_vector_type& {
		return children_;
	}

	[[nodiscard]] constexpr auto depths() noexcept -> depths_vector_type& {
		return depths_;
	}

	[[nodiscard]] constexpr auto depths() const noexcept -> const depths_vector_type& {
		return depths_;
	}

	void add_child_to(std::size_t to, std::size_t child_idx) {
		children_[to].push_back(child_idx);
	}

private:
	blocks_vector_type blocks_;
	children_vector_type children_;
	depths_vector_type depths_;
};

}	 // namespace cui::scenes

#endif	  // constexpr Scene

#endif	  // CUI_SCENES_SCENE_HPP
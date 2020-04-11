#ifndef CUI_NARY_TREE_NODE_HPP
#define CUI_NARY_TREE_NODE_HPP

#include <vector>
#include <functional>
#include <aliases.hpp>

namespace cui::nary {

template <typename T>
class Node
{
public:
    using data_type = T;
    using size_type = u64;
    using vec_t = std::vector<size_type>;

    [[nodiscard]] auto data() noexcept -> data_type& {
        return data_;
    }
    
    [[nodiscard]] auto data() const noexcept -> const data_type& {
        return data_;
    }
    
    [[nodiscard]] auto children() noexcept -> vec_t& {
        return children_;
    }
    
    [[nodiscard]] auto children() const noexcept -> const vec_t& {
        return children_;
    }
    
    [[nodiscard]] auto depth() noexcept -> size_type& {
        return depth_;
    }
    
    [[nodiscard]] auto depth() const noexcept -> const size_type {
        return depth_;
    }

private:
    data_type data_;
    vec_t children_;
    size_type depth_;
};

}     // namespace cui

#endif      // CUI_NARY_TREE_NODE_HPP
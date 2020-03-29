#ifndef CUI_DATA_TYPES_VECTOR_HPP
#define CUI_DATA_TYPES_VECTOR_HPP

#include <array>

#include <global/constants.hpp>

namespace cui::styles {

template <typename T, std::size_t N>
using Vector_ = std::array<T, N>;

using Vector2f = Vector_<float, 2>;
using Vector3f = Vector_<float, 3>;
using Vector4f = Vector_<float, 4>;

using Vector2i = Vector_<int, 2>;
using Vector3i = Vector_<int, 3>;
using Vector4i = Vector_<int, 4>;

}	 // namespace cui::styles

#endif	  // CUI_DATA_TYPES_VECTOR_HPP
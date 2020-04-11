#ifndef CUI_GET_PATH_HEAD_HPP
#define CUI_GET_PATH_HEAD_HPP

#include <utils/min.hpp>
#include <string>

namespace cui {

auto get_path_head(const std::string& path) {
	const auto _1 = path.find_last_of('/');
	const auto _2 = path.find_last_of('\\');
	const auto idx = min(_1, _2);

	return path.substr(idx + 1);
}

}	 // namespace cui

#endif	  // CUI_GET_PATH_HEAD_HPP
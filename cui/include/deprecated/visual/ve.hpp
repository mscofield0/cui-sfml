#ifndef CUI_VE_HPP
#define CUI_VE_HPP

#include <string>
#include "../tsl/hopscotch_map.h"

namespace cui {

struct ve {
	tsl::hopscotch_map<std::string, std::function<void()>> event;


};

}

#endif //CUI_VE_HPP

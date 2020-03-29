#ifndef CUI_COMPILE_TIME_EXCEPTION_HPP
#define CUI_COMPILE_TIME_EXCEPTION_HPP

namespace cui {

template <typename StringT>
class ExceptionCT {
public:
    ExceptionCT(StringT str) : what_(str) {}

    StringT what() const {
        return what_;
    }

private:
    StringT what_;
};

}

#endif // CUI_COMPILE_TIME_EXCEPTION_HPP
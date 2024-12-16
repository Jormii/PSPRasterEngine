#ifndef T_HPP
#define T_HPP

#include <stdio.h>

#include <type_traits>

template <typename T, typename EC>
struct Result {
    T value;
    const EC err_code;

    static_assert(std::is_enum_v<EC>);

    Result(T value, const EC err_code) : value(value), err_code(err_code) {}

    bool ok() const {
        return (int)(this->err_code) == 0;
    }
};

enum class DoubleEC {
    OK,
    NOT_EVEN
};

Result<int, DoubleEC> double_if_even_result(int n);

#endif

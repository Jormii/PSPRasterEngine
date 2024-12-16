#include "t.hpp"

#include <stdio.h>

Result<int, DoubleEC> double_if_even_result(int n) {
    
    printf("%d\n", __LINE__);
    printf("%s\n", __FILE__);

    using Result = Result<int, DoubleEC>;

    if ((n % 2) != 0) {
        return Result(0, DoubleEC::NOT_EVEN);
    }

    return Result(2 * n, DoubleEC::OK);
}

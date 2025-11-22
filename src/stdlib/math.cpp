#include "math.hpp"

namespace chtholly::stdlib {

StdlibModule get_math_module() {
    return {
        math_module,
        {"cmath"}
    };
}

}

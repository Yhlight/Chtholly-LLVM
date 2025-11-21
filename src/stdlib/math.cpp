#include "math.hpp"

namespace chtholly::stdlib {

StdlibModule get_math_module() {
    return {
        R"(
namespace math {
    double sqrt(double n) {
        return std::sqrt(n);
    }

    double pow(double base, double exp) {
        return std::pow(base, exp);
    }

    double sin(double n) {
        return std::sin(n);
    }

    double cos(double n) {
        return std::cos(n);
    }

    double tan(double n) {
        return std::tan(n);
    }
}
)",
        {"cmath"}
    };
}

} // namespace chtholly::stdlib

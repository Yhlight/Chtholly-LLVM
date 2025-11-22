#pragma once

#include <string>
#include <cmath>
#include "../Stdlib.hpp"

namespace chtholly {
namespace stdlib {

StdlibModule get_math_module();

const std::string math_module = R"(
namespace math {
    double sqrt(double n) { return std::sqrt(n); }
    double pow(double base, double exp) { return std::pow(base, exp); }
    double sin(double n) { return std::sin(n); }
    double cos(double n) { return std::cos(n); }
    double tan(double n) { return std::tan(n); }
    double abs(double n) { return std::abs(n); }
    double floor(double n) { return std::floor(n); }
    double ceil(double n) { return std::ceil(n); }
    double round(double n) { return std::round(n); }
    double log(double n) { return std::log(n); }
    double log10(double n) { return std::log10(n); }
    double exp(double n) { return std::exp(n); }
}
)";

} // namespace stdlib
} // namespace chtholly

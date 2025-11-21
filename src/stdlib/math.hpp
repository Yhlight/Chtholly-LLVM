#ifndef CHTHOLLY_STDLIB_MATH_HPP
#define CHTHOLLY_STDLIB_MATH_HPP

#include "../Stdlib.hpp"
#include <string>

namespace chtholly::stdlib {

/**
 * @brief Returns the C++ source code for the Chtholly 'math' module.
 *
 * This function provides the necessary C++ implementation for the standard
 * math functions.
 * @return A StdlibModule struct containing the module's source and headers.
 */
StdlibModule get_math_module();

} // namespace chtholly::stdlib

#endif // CHTHOLLY_STDLIB_MATH_HPP

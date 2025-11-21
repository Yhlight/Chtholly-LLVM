#ifndef CHTHOLLY_STDLIB_IOSTREAM_HPP
#define CHTHOLLY_STDLIB_IOSTREAM_HPP

#include "../Stdlib.hpp"
#include <string>

namespace chtholly::stdlib {

/**
 * @brief Returns the C++ source code for the Chtholly 'iostream' module.
 *
 * This function provides the necessary C++ implementation for the standard
 * iostream functions, which can then be injected into the final transpiled output.
 * @return A StdlibModule struct containing the module's source and headers.
 */
StdlibModule get_iostream_module();

} // namespace chtholly::stdlib

#endif // CHTHOLLY_STDLIB_IOSTREAM_HPP

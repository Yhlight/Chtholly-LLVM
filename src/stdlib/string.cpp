#include "string.hpp"

namespace chtholly::stdlib {

StdlibModule get_string_module() {
    return {
        string_module,
        {"string", "vector", "algorithm", "cctype", "stdexcept"}
    };
}

}

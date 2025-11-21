#include "Stdlib.hpp"
#include "stdlib/iostream.hpp"
#include "stdlib/string.hpp"

namespace chtholly {

Stdlib::Stdlib() {
    initialize_modules();
}

void Stdlib::initialize_modules() {
    modules["iostream"] = stdlib::get_iostream_module;
    modules["string"] = stdlib::get_string_module;
    // Add other standard library modules here in the future
}

bool Stdlib::has_module(const std::string& name) const {
    return modules.count(name) > 0;
}

std::optional<std::string> Stdlib::get_module_source(const std::string& name) const {
    auto it = modules.find(name);
    if (it != modules.end()) {
        return it->second();
    }
    return std::nullopt;
}

} // namespace chtholly

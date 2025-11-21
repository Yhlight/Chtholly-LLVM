#ifndef CHTHOLLY_STDLIB_HPP
#define CHTHOLLY_STDLIB_HPP

#include <string>
#include <unordered_map>
#include <functional>
#include <optional>

namespace chtholly {

class Stdlib {
public:
    Stdlib();

    /**
     * @brief Checks if a standard library module with the given name exists.
     * @param name The name of the module.
     * @return True if the module exists, false otherwise.
     */
    bool has_module(const std::string& name) const;

    /**
     * @brief Gets the source code of a standard library module.
     * @param name The name of the module.
     * @return An optional containing the module's source code if found.
     */
    std::optional<std::string> get_module_source(const std::string& name) const;

private:
    using ModuleSourceProvider = std::function<std::string()>;
    std::unordered_map<std::string, ModuleSourceProvider> modules;

    void initialize_modules();
};

} // namespace chtholly

#endif // CHTHOLLY_STDLIB_HPP

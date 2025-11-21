#ifndef CHTHOLLY_STDLIB_HPP
#define CHTHOLLY_STDLIB_HPP

#include <string>
#include <unordered_map>
#include <functional>
#include <optional>
#include <vector>

namespace chtholly {

struct StdlibModule {
    std::string source;
    std::vector<std::string> headers;
};

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
     * @brief Gets a standard library module by name.
     * @param name The name of the module.
     * @return An optional containing the module if found.
     */
    std::optional<StdlibModule> get_module(const std::string& name) const;

private:
    using ModuleProvider = std::function<StdlibModule()>;
    std::unordered_map<std::string, ModuleProvider> modules;

    void initialize_modules();
};

} // namespace chtholly

#endif // CHTHOLLY_STDLIB_HPP

#ifndef CHTHOLLY_TYPE_HPP
#define CHTHOLLY_TYPE_HPP

#include <string>
#include <memory>
#include <utility>

namespace chtholly {

enum class TypeKind {
    PRIMITIVE,
    ARRAY
};

struct Type {
    virtual ~Type() = default;
    virtual TypeKind getKind() const = 0;
};

struct PrimitiveType : public Type {
    explicit PrimitiveType(std::string name) : name(std::move(name)) {}
    TypeKind getKind() const override { return TypeKind::PRIMITIVE; }

    const std::string name;
};

struct ArrayType : public Type {
    explicit ArrayType(std::shared_ptr<Type> element_type) : element_type(std::move(element_type)) {}
    TypeKind getKind() const override { return TypeKind::ARRAY; }

    const std::shared_ptr<Type> element_type;
};

} // namespace chtholly

#endif // CHTHOLLY_TYPE_HPP

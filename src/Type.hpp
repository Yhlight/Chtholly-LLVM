#ifndef CHTHOLLY_TYPE_HPP
#define CHTHOLLY_TYPE_HPP

#include <string>
#include <memory>
#include <utility>
#include <vector>

namespace chtholly {

enum class TypeKind {
    PRIMITIVE,
    ARRAY,
    ENUM,
    FUNCTION
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

struct EnumType : public Type {
    explicit EnumType(std::string name) : name(std::move(name)) {}
    TypeKind getKind() const override { return TypeKind::ENUM; }

    const std::string name;
};

struct FunctionType : public Type {
    FunctionType(std::vector<std::shared_ptr<Type>> param_types, std::shared_ptr<Type> return_type)
        : param_types(std::move(param_types)), return_type(std::move(return_type)) {}
    TypeKind getKind() const override { return TypeKind::FUNCTION; }

    const std::vector<std::shared_ptr<Type>> param_types;
    const std::shared_ptr<Type> return_type;
};

} // namespace chtholly

#endif // CHTHOLLY_TYPE_HPP

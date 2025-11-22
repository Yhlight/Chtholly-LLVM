#ifndef CHTHOLLY_TYPE_HPP
#define CHTHOLLY_TYPE_HPP

#include <string>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

namespace chtholly {

enum class TypeKind {
    PRIMITIVE,
    ARRAY,
    ENUM,
    FUNCTION,
    REFERENCE
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
    explicit ArrayType(std::shared_ptr<Type> element_type, std::optional<int> size = std::nullopt)
        : element_type(std::move(element_type)), size(size) {}
    TypeKind getKind() const override { return TypeKind::ARRAY; }

    const std::shared_ptr<Type> element_type;
    const std::optional<int> size;
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

enum class ReferenceKind {
    IMMUTABLE,
    MUTABLE,
    MOVE,
    COPY
};

struct ReferenceType : public Type {
    ReferenceType(std::shared_ptr<Type> referenced_type, ReferenceKind kind)
        : referenced_type(std::move(referenced_type)), kind(kind) {}
    TypeKind getKind() const override { return TypeKind::REFERENCE; }

    const std::shared_ptr<Type> referenced_type;
    const ReferenceKind kind;
};

} // namespace chtholly

#endif // CHTHOLLY_TYPE_HPP

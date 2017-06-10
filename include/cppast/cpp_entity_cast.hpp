// Copyright (C) 2017 Manu Sanchez <Manu343726@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef CPPAST_CPP_ENTITY_CAST_HPP_INCLUDED
#define CPPAST_CPP_ENTITY_CAST_HPP_INCLUDED

#include <cppast/cpp_entity.hpp>
#include <type_safe/strong_typedef.hpp>
#include <stdexcept>
#include <typeinfo>

namespace cppast
{

/// Exception thrown when a [cppast::cpp_entity_cast()]() operation on a reference
/// fails.
struct bad_cpp_entity_cast : public type_safe::strong_typedef<bad_cpp_entity_cast, std::bad_cast>
{
    using type_safe::strong_typedef<bad_cpp_entity_cast, std::bad_cast>::strong_typedef;
};

/// Safely converts type-erased entity pointers to the cpp entity
/// class representing that entity kind
/// \returns A pointer to the entity type object if the given entity has the
/// kind represented by the entity type. Returns nullptr otherwise.
template<typename Entity>
const Entity* cpp_entity_cast(const cpp_entity* entity) noexcept
{
    static_assert(std::is_base_of<cpp_entity, Entity>::value,
        "Cannot cast to a non cpp_entity class?");

    if(entity != nullptr && entity->kind() == Entity::kind())
    {
        return static_cast<const Entity*>(entity);
    }
    else
    {
        return nullptr;
    }
}

/// Safely converts type-erased entity references to the cpp entity
/// class representing that entity kind
/// \returns A reference to the entity type object if the given entity has the
/// kind represented by the entity type. Else throws [cppast::bad_cpp_entity_cast]().
template<typename Entity>
const Entity& cpp_entity_cast(const cpp_entity& entity)
{
    auto* result = cpp_entity_cast<Entity>(&entity);

    if(result != nullptr)
    {
        return *result;
    }
    else
    {
        throw bad_cpp_entity_cast{};
    }
}

/// Safely converts type-erased entity pointers to the cpp entity
/// class representing that entity kind
/// \returns A pointer to the entity type if the given entity has the
/// kind represented by the entity type. Returns nullptr otherwise.
template<typename Entity>
Entity* cpp_entity_cast(cpp_entity* entity) noexcept
{
    return const_cast<Entity*>(cpp_entity_cast<Entity>(const_cast<const cpp_entity*>(entity)));
}

/// Safely converts type-erased entity references to the cpp entity
/// class representing that entity kind
/// \returns A reference to the entity type object if the given entity has the
/// kind represented by the entity type. Else throws [cppast::bad_cpp_entity_cast]().
template<typename Entity>
Entity& cpp_entity_cast(cpp_entity& entity)
{
    return const_cast<Entity&>(cpp_entity_cast<Entity>(const_cast<const cpp_entity&>(entity)));
}

} // namespace cppast

#endif // CPPAST_CPP_ENTITY_CAST_HPP_INCLUDED

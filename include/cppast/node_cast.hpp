// Copyright (C) 2017 Manu Sanchez <Manu343726@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef CPPAST_NODE_CAST_HPP_INCLUDED
#define CPPAST_NODE_CAST_HPP_INCLUDED

#include <type_safe/strong_typedef.hpp>
#include <stdexcept>
#include <typeinfo>

namespace cppast
{

/// Exception thrown when a [cppast::node_cast()]() operation on a reference
/// fails.
struct bad_node_cast : public type_safe::strong_typedef<bad_node_cast, std::bad_cast>
{
    using type_safe::strong_typedef<bad_node_cast, std::bad_cast>::strong_typedef;
};

namespace detail
{

template<typename T>
using void_t = std::conditional_t<true, void, T>;

template<typename T, typename = void>
struct kind_traits
{
    using kind_type = int;

    static_assert(sizeof(T) != sizeof(T),
        "The given type is not a supported AST node type");
};

template<typename T, typename = void>
struct class_kind_traits
{
    using kind_type = int;

    static_assert(sizeof(T) != sizeof(T),
        "The given type is not a supported AST node type");
};

template<typename T>
struct kind_traits<T, void_t<
    decltype(std::declval<T>().kind())
>>
{
    using kind_type = decltype(std::declval<T>().kind());

    static kind_type kind(const T& node)
    {
        return node.kind();
    }
};

template<typename T>
struct class_kind_traits<T, void_t<
    decltype(T::kind())
>>
{
    using kind_type = decltype(T::kind());

    static kind_type kind()
    {
        return T::kind();
    }
};

template<typename Base, typename Node, typename = void>
struct node_cast
{
    static const Node* apply(const Base*)
    {
        return nullptr;
    }
};

template<typename Base, typename Node>
struct node_cast<Base, Node, std::enable_if_t<
    std::is_base_of<Base, Node>::value &&
    std::is_same<
        typename kind_traits<Base>::kind_type,
        typename class_kind_traits<Node>::kind_type
    >::value
>>
{
    static const Node* apply(const Base* node)
    {
        if(kind_traits<Base>::kind(*node) == class_kind_traits<Node>::kind())
        {
            return static_cast<const Node*>(node);
        }
        else
        {
            return nullptr;
        }
    }
};

} // namespace cppast::detail


/// Safely converts type-erased node pointers to the cpp node
/// class representing that node kind
/// \returns A pointer to the node type object if the given node has the
/// kind represented by the node type. Returns nullptr otherwise.
template<typename Node, typename Base>
const Node* node_cast(const Base* node) noexcept
{
    return ::cppast::detail::node_cast<Base, Node>::apply(node);
}

/// Safely converts type-erased node references to the cpp node
/// class representing that node kind
/// \returns A reference to the node type object if the given node has the
/// kind represented by the node type. Else throws [cppast::bad_node_cast]().
template<typename Node, typename Base>
const Node& node_cast(const Base& node)
{
    auto* result = node_cast<Node>(&node);

    if(result != nullptr)
    {
        return *result;
    }
    else
    {
        throw bad_node_cast{};
    }
}

/// Safely converts type-erased node pointers to the cpp node
/// class representing that node kind
/// \returns A pointer to the node type if the given node has the
/// kind represented by the node type. Returns nullptr otherwise.
template<typename Node, typename Base>
Node* node_cast(Base* node) noexcept
{
    return const_cast<Node*>(node_cast<Node>(const_cast<const Base*>(node)));
}

/// Safely converts type-erased node references to the cpp node
/// class representing that node kind
/// \returns A reference to the node type object if the given node has the
/// kind represented by the node type. Else throws [cppast::bad_node_cast]().
template<typename Node, typename Base>
Node& node_cast(Base& node)
{
    return const_cast<Node&>(node_cast<Node>(const_cast<const Base&>(node)));
}

} // namespace cppast

#endif // CPPAST_NODE_CAST_HPP_INCLUDED

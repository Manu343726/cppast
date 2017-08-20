// Copyright (C) 2017 Manu Sanchez <Manu343726@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef CPPAST_MATCHERS_NODE_HPP_INCLUDED
#define CPPAST_MATCHERS_NODE_HPP_INCLUDED

#include <cppast/cpp_entity.hpp>
#include <cppast/cpp_expression.hpp>
#include <cppast/cpp_type.hpp>
#include <cppast/node_cast.hpp>
#include <cppast/detail/parser/ast.hpp>
#include <cppast/detail/utils/overloaded_function.hpp>
#include <type_safe/variant.hpp>
#include <type_safe/visitor.hpp>

namespace cppast
{

namespace matchers
{

namespace detail
{

/// Holds the set of supported AST hierarchies
using node_variant = type_safe::variant<
    const cppast::cpp_entity*,
    const cppast::cpp_expression*,
    const cppast::cpp_type*,
    const cppast::detail::parser::ast::node*
>;

} // namespace cppast::matchers::detail

/// A reference to an AST node
/// \notes This alias provides a generic
/// interface to the three different AST node
/// hierarchies supported.
class node : public detail::node_variant
{
public:
    using detail::node_variant::node_variant;

    /// Returns the AST node ref converted to the required AST node type,
    /// if the given type is the dynamic type of the node.
    template<typename Node>
    type_safe::optional_ref<const Node> as() const
    {
        if(detail::node_variant::has_value())
        {
            return visit(
                [this](const auto& node) -> type_safe::optional_ref<const Node>
                {
                    const auto* node_ptr = cppast::node_cast<Node>(&node);

                    if(node_ptr != nullptr)
                    {
                        return type_safe::opt_ref(node_ptr);
                    }
                    else
                    {
                        return type_safe::nullopt;
                    }
                }
            );
        }
        else
        {
            return type_safe::nullopt;
        }
    }

    /// Invokes the given callback overload with the
    /// node, after casting it to the right AST node
    /// hierarchy.
    template<typename... Visitors>
    auto visit(Visitors&&... visitors) const
    {
        auto visitor = cppast::detail::utils::overloaded_function(
            std::forward<Visitors>(visitors)...
        );

        return type_safe::visit(
            [this, &visitor](const auto* node_ptr)
            {
                return visitor(*node_ptr);
            },
            static_cast<const detail::node_variant&>(*this)
        );
    }
};

/// Casts the given AST node ref to the required AST node type,
/// if the given type is the dynamic type of the node.
template<typename Node>
type_safe::optional_ref<const Node> node_cast(const node& node)
{
    return node.as<Node>();
}

} // namespace cppast::matchers

} // namespace cppast

#endif // CPPAST_MATCHERS_NODE_HPP_INCLUDED

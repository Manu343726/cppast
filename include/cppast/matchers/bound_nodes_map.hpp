// Copyright (C) 2017 Manu Sanchez <Manu343726@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef CPPAST_MATCHERS_BOUND_NODES_MAP_HPP_INCLUDED
#define CPPAST_MATCHERS_BOUND_NODES_MAP_HPP_INCLUDED

#include <cppast/matchers/node.hpp>
#include <type_safe/reference.hpp>
#include <string>
#include <vector>
#include <unordered_map>

namespace cppast
{

namespace matchers
{


/// Set of AST nodes bound to a match
class bound_nodes_map
{
public:
    /// \effects Binds a matched node to the map using the given id
    void bind(const std::string& id, const node& node);

    /// \effects Binds a matched node to the map using the given id
    template<typename Node>
    void bind(const std::string& id, const Node& node)
    {
        bind(id, matchers::node{&node});
    }

    /// \returns A reference to the node bound to the given id, if any
    type_safe::optional_ref<const node> get(const std::string& id) const;

    /// \returns A reference to the node bound to the given id, if any, casted to
    /// the right Node type
    template<typename Node>
    type_safe::optional_ref<const Node> get_as(const std::string& id) const
    {
        auto ref = get(id);

        if(ref.has_value())
        {
            return node_cast<Node>(ref.value());
        }
        else
        {
            return type_safe::nullopt;
        }
    }

    /// \returns The set of references to the nodes bound to the given id.
    /// If no node is bound to that id an empty set is returned.
    std::vector<node> get_all(const std::string& id) const;

private:
    std::unordered_map<std::string, std::vector<node>> _nodes;
};

} // namespace cppast::matchers

} // namespace cppast

#endif // CPPAST_MATCHERS_BOUND_NODES_MAP_H

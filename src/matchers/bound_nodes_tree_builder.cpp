// Copyright (C) 2017 Manu Sanchez <Manu343726@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <cppast/matchers/bound_nodes_tree_builder.hpp>

using namespace cppast;
using namespace cppast::matchers;

void bound_nodes_tree_builder::bind(const std::string& id, const cpp_entity& node)
{
    if(_bindings.empty())
    {
        _bindings.push_back({});
    }

    for(auto& branch : _bindings)
    {
        branch.bind(id, node);
    }
}

void bound_nodes_tree_builder::visit_matches(bound_nodes_tree_builder::visitor& visitor)
{
    for(auto& branch : _bindings)
    {
        visitor.visitMatch(branch);
    }
}

void bound_nodes_tree_builder::add_match(const bound_nodes_tree_builder& tree)
{
    for(const auto& branch : tree._bindings)
    {
        _bindings.push_back(branch);
    }
}

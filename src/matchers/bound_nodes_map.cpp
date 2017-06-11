// Copyright (C) 2017 Manu Sanchez <Manu343726@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <cppast/matchers/bound_nodes_map.hpp>
#include <debug_assert.hpp>

using namespace cppast;
using namespace cppast::matchers;
using namespace type_safe;

void bound_nodes_map::bind(const std::string& id, const cpp_entity& node)
{
    _nodes[id].emplace_back(node);
}

std::vector<object_ref<const cpp_entity>> bound_nodes_map::get_all(const std::string& id) const
{
    auto it = _nodes.find(id);

    if(it != _nodes.end())
    {
        return it->second;
    }
    else
    {
        return {};
    }
}

optional_ref<const cpp_entity> bound_nodes_map::get(const std::string& id) const
{
    const auto& set = get_all(id);

    if(set.empty())
    {
        return {};
    }
    else
    {
        return set.front();
    }
}

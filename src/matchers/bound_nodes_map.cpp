// Copyright (C) 2017 Manu Sanchez <Manu343726@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <cppast/matchers/bound_nodes_map.hpp>
#include <cppast/detail/assert.hpp>

using namespace cppast;
using namespace cppast::matchers;
using namespace type_safe;

void bound_nodes_map::bind(const std::string& id, const node& node)
{
    DEBUG_ASSERT(node.has_value(), cppast::detail::assert_handler{});
    _nodes[id].emplace_back(node);
    DEBUG_ASSERT(_nodes[id].back().has_value(), cppast::detail::assert_handler{});
}

std::vector<node> bound_nodes_map::get_all(const std::string& id) const
{
    auto it = _nodes.find(id);

    if(it != _nodes.end())
    {
        for(const auto& node : it->second)
        {
            DEBUG_ASSERT(node.has_value(), cppast::detail::assert_handler{});
        }

        return it->second;
    }
    else
    {
        return {};
    }
}

optional_ref<const node> bound_nodes_map::get(const std::string& id) const
{
    const auto& set = get_all(id);

    if(set.empty())
    {
        return type_safe::nullopt;
    }
    else
    {
        DEBUG_ASSERT(set.front().has_value(), cppast::detail::assert_handler{});
        return type_safe::opt_ref(type_safe::ref(set.front()));
    }
}

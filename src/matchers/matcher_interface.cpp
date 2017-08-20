// Copyright (C) 2017 Manu Sanchez <Manu343726@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <cppast/matchers/matcher_interface.hpp>

using namespace cppast;
using namespace cppast::matchers;

bool type_erased_matcher::matches(const node& node, ast_match_finder& finder, bound_nodes_tree_builder& builder) const
{
    return _impl->matches(node, finder, builder);
}

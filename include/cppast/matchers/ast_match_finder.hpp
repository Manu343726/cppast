// Copyright (C) 2017 Manu Sanchez <Manu343726@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef CPPAST_MATCHERS_AST_MATCH_FINDER_INCLUDED
#define CPPAST_MATCHERS_AST_MATCH_FINDER_INCLUDED

#include <cppast/matchers/bound_nodes_tree_builder.hpp>
#include <cppast/matchers/matcher_interface.hpp>

namespace cppast
{

namespace matchers
{

class ast_match_finder
{
public:
    virtual ~ast_match_finder() = default;

    virtual bool matches_ancestor(
        basic_matcher_interface& matcher,
        const cpp_entity& current,
        bound_nodes_tree_builder& builder,
        std::size_t max_depth
    ) const = 0;

    virtual bool matches_descendant(
        basic_matcher_interface& matcher,
        const cpp_entity& current,
        bound_nodes_tree_builder& builder,
        std::size_t max_depth
    ) const = 0;

    bool matches_parent(
        basic_matcher_interface& matcher,
        const cpp_entity& current,
        bound_nodes_tree_builder& builder
    ) const;

    bool matches_child(
        basic_matcher_interface& matcher,
        const cpp_entity& current,
        bound_nodes_tree_builder& builder
    ) const;
};

}

}

#endif // CPPAST_MATCHERS_AST_MATCH_FINDER_INCLUDED

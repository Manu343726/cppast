// Copyright (C) 2017 Manu Sanchez <Manu343726@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef CPPAST_MATCHERS_BOUND_NODES_TREE_BUILDER_INCLUDED
#define CPPAST_MATCHERS_BOUND_NODES_TREE_BUILDER_INCLUDED

#include "bound_nodes_map.hpp"
#include <algorithm>
#include <vector>
#include <string>

namespace cppast
{

namespace matchers
{

/// Builds a tree of sub-matched nodes
///
/// This class collects the different nodes
/// bound by matchers and its sub-matchers
/// across the AST traversal. Each match has its
/// own set of bound nodes,
class bound_nodes_tree_builder
{
public:
    class visitor
    {
    public:
        virtual ~visitor() = default;
        virtual void visitMatch(const bound_nodes_map& bound_nodes) = 0;
    };

    void bind(const std::string& id, const node& node);
    template<typename Node>
    void bind(const std::string& id, const Node& node)
    {
        bind(id, matchers::node{node});
    }
    void visit_matches(visitor& visitor);
    void add_match(const bound_nodes_tree_builder& tree);

    template<typename Predicate>
    bool remove_bindings(Predicate&& predicate)
    {
        _bindings.erase(
            std::remove_if(_bindings.begin(), _bindings.end(), std::forward<Predicate>(predicate)),
            _bindings.end()
        );

        return !_bindings.empty();
    }

private:
    std::vector<bound_nodes_map> _bindings;
};

} // namespace cppast::matchers

} // namespace cppast

#endif // CPPAST_MATCHERS_BOUND_NODES_TREE_BUILDER_INCLUDED

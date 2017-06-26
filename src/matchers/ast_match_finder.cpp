#include <cppast/matchers/ast_match_finder.hpp>

using namespace cppast;
using namespace cppast::matchers;

bool ast_match_finder::matches_parent(
    basic_matcher_interface& matcher,
    const cpp_entity& current,
    bound_nodes_tree_builder& builder
) const
{
    return matches_ancestor(
        matcher,
        current,
        builder,
        1
    );
}

bool ast_match_finder::matches_child(
    basic_matcher_interface& matcher,
    const cpp_entity& current,
    bound_nodes_tree_builder& builder
) const
{
    return matches_descendant(
        matcher,
        current,
        builder,
        1
    );
}

// Copyright (C) 2017 Manu Sanchez <Manu343726@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <cppast/matchers/variadic_matcher.hpp>
#include <cppast/detail/assert.hpp>
#include <numeric>

using namespace cppast;
using namespace cppast::matchers;

namespace variadic_functions
{

bool all_of(const node& node, ast_match_finder& finder, bound_nodes_tree_builder& builder, const std::vector<type_erased_matcher>& inner_matchers);
bool any_of(const node& node, ast_match_finder& finder, bound_nodes_tree_builder& builder, const std::vector<type_erased_matcher>& inner_matchers);
bool each_of(const node& node, ast_match_finder& finder, bound_nodes_tree_builder& builder, const std::vector<type_erased_matcher>& inner_matchers);
bool none_of(const node& node, ast_match_finder& finder, bound_nodes_tree_builder& builder, const std::vector<type_erased_matcher>& inner_matchers);

}

using variadic_matcher_function = bool(*)(const node&, ast_match_finder&, bound_nodes_tree_builder&, const std::vector<type_erased_matcher>&);

template<variadic_matcher_function Func>
class variadic_matcher_impl : public basic_matcher_interface
{
public:
    bool matches(const node& node, ast_match_finder& finder, bound_nodes_tree_builder& builder) const override final
    {
        return Func(node, finder, builder, _inner_matchers);
    }

    static type_erased_matcher make(std::vector<type_erased_matcher> inner_matchers)
    {
        return {variadic_matcher_impl{std::move(inner_matchers)}};
    }

private:
    std::vector<type_erased_matcher> _inner_matchers;

    variadic_matcher_impl(std::vector<type_erased_matcher> inner_matchers) :
        _inner_matchers(std::move(inner_matchers))
    {}
};

type_erased_matcher variadic_matcher_impl_for(variadic_matcher::variadic_operator op, std::vector<type_erased_matcher> inner_matchers)
{
    switch(op)
    {
    case variadic_matcher::variadic_operator::all_of:
        return variadic_matcher_impl<variadic_functions::all_of>::make(std::move(inner_matchers));
    case variadic_matcher::variadic_operator::any_of:
        return variadic_matcher_impl<variadic_functions::any_of>::make(std::move(inner_matchers));
    case variadic_matcher::variadic_operator::each_of:
        return variadic_matcher_impl<variadic_functions::each_of>::make(std::move(inner_matchers));
    case variadic_matcher::variadic_operator::none_of:
        return variadic_matcher_impl<variadic_functions::none_of>::make(std::move(inner_matchers));
    }

    DEBUG_UNREACHABLE(cppast::detail::assert_handler{});
    throw;
}

variadic_matcher::variadic_matcher(variadic_matcher::variadic_operator op, std::vector<type_erased_matcher> inner_matchers) :
    _matcher{variadic_matcher_impl_for(op, std::move(inner_matchers))}
{}

bool variadic_matcher::matches(const node& node, ast_match_finder& finder, bound_nodes_tree_builder& builder) const
{
    return _matcher.matches(node, finder, builder);
}

namespace variadic_functions
{

bool all_of(const node& node, ast_match_finder& finder, bound_nodes_tree_builder& builder, const std::vector<type_erased_matcher>& inner_matchers)
{
    return std::all_of(inner_matchers.begin(), inner_matchers.end(), [&node, &finder, &builder](const type_erased_matcher& matcher)
    {
        return matcher.matches(node, finder, builder);
    });
}

bool any_of(const node& node, ast_match_finder& finder, bound_nodes_tree_builder& builder, const std::vector<type_erased_matcher>& inner_matchers)
{
    return std::any_of(inner_matchers.begin(), inner_matchers.end(), [&node, &finder, &builder](const type_erased_matcher& matcher)
    {
        return matcher.matches(node, finder, builder);
    });
}

bool each_of(const node& node, ast_match_finder& finder, bound_nodes_tree_builder& builder, const std::vector<type_erased_matcher>& inner_matchers)
{
    bool result = false;

    for(const auto& matcher : inner_matchers)
    {
        // No short-circuiting, all matchers are evaluated
        result |= matcher.matches(node, finder, builder);
    }

    return result;
}

bool none_of(const node& node, ast_match_finder& finder, bound_nodes_tree_builder&, const std::vector<type_erased_matcher>& inner_matchers)
{
    return std::none_of(inner_matchers.begin(), inner_matchers.end(), [&node, &finder](const type_erased_matcher& matcher)
    {
        // Ignore nodes bound by matching inner matchers,
        // since the result is negative
        bound_nodes_tree_builder dummy_builder;
        return matcher.matches(node, finder, dummy_builder);
    });
}

}

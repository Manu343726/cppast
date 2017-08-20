// Copyright (C) 2017 Manu Sanchez <Manu343726@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef CPPAST_MATCHERS_VARIADIC_MATCHER_INCLUDED
#define CPPAST_MATCHERS_VARIADIC_MATCHER_INCLUDED

#include <cppast/matchers/matcher_interface.hpp>
#include <vector>

namespace cppast
{

namespace matchers
{

class variadic_matcher : public basic_matcher_interface
{
public:
    enum class variadic_operator
    {
        all_of,
        any_of,
        each_of,
        none_of
    };

    variadic_matcher(variadic_operator op, std::vector<type_erased_matcher> matchers);

    template<typename... Matchers>
    variadic_matcher(const variadic_operator op, Matchers&&... inner_matchers) :
        variadic_matcher{
            op,
            std::vector<type_erased_matcher>{
                type_erased_matcher{std::forward<Matchers>(inner_matchers)}...
            }
        }
    {}

    bool matches(const node& node, ast_match_finder& finder, bound_nodes_tree_builder& builder) const override final;

private:
    type_erased_matcher _matcher;
};

template<typename... Matchers>
variadic_matcher all_of(Matchers&&... inner_matchers)
{
    return {variadic_matcher::variadic_operator::all_of, std::forward<Matchers>(inner_matchers)...};
}

template<typename... Matchers>
variadic_matcher any_of(Matchers&&... inner_matchers)
{
    return {variadic_matcher::variadic_operator::any_of, std::forward<Matchers>(inner_matchers)...};
}

template<typename... Matchers>
variadic_matcher each_of(Matchers&&... inner_matchers)
{
    return {variadic_matcher::variadic_operator::each_of, std::forward<Matchers>(inner_matchers)...};
}

template<typename... Matchers>
variadic_matcher none_of(Matchers&&... inner_matchers)
{
    return {variadic_matcher::variadic_operator::none_of, std::forward<Matchers>(inner_matchers)...};
}

template<typename Matcher>
variadic_matcher unless(Matcher&& inner_matcher)
{
    return none_of(std::forward<Matcher>(inner_matcher));
}

}

}

#endif // CPPAST_MATCHERS_VARIADIC_MATCHER_INCLUDED

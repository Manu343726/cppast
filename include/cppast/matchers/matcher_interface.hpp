// Copyright (C) 2017 Manu Sanchez <Manu343726@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef CPPAST_MATCHERS_MATCHER_INTERFACE_INCLUDED
#define CPPAST_MATCHERS_MATCHER_INTERFACE_INCLUDED

#include <cppast/cpp_entity_cast.hpp>
#include <cppast/matchers/bound_nodes_tree_builder.hpp>
#include <memory>
#include <type_traits>

namespace cppast
{

namespace matchers
{

class ast_match_finder;

class basic_matcher_interface
{
public:
    virtual bool matches(const cpp_entity& node, ast_match_finder& finder, bound_nodes_tree_builder& builder) const = 0;
};

template<typename Node>
class matcher_interface : public basic_matcher_interface
{
public:
    virtual bool matches(const Node& node, ast_match_finder& finder, bound_nodes_tree_builder& builder) const = 0;

private:
    bool matches(const cpp_entity& node, ast_match_finder& finder, bound_nodes_tree_builder& builder) const override final
    {
        const auto* node_ = cppast::cpp_entity_cast<Node>(&node);

        if(node_ != nullptr)
        {
            return matcher_interface::matches(*node_, finder, builder);
        }
        else
        {
            return false;
        }
    }
};

class type_erased_matcher : public basic_matcher_interface
{
public:
    template<typename Matcher>
    type_erased_matcher(Matcher&& impl) :
        _impl{new typename std::decay<Matcher>::type{std::forward<Matcher>(impl)}}
    {}

    bool matches(const cpp_entity& node, ast_match_finder& finder, bound_nodes_tree_builder& builder) const override final;

private:
    std::shared_ptr<basic_matcher_interface> _impl;
};

}

}

#endif // CPPAST_MATCHERS_MATCHER_INTERFACE_INCLUDED

// Copyright (C) 2017 Manu Sanchez <Manu343726@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <catch.hpp>
#include <cppast/matchers/bound_nodes_map.hpp>


#include <cppast/cpp_class.hpp>
#include <cppast/cpp_type_alias.hpp>
#include "../test_parser.hpp"

using namespace cppast;
using namespace cppast::matchers;

TEST_CASE("bound_nodes_map")
{
    auto code = R"(

/// using mytypedef=void;
using mytypedef = void;

/// class foo{
/// };
class foo{};
)";

    cpp_entity_index idx;
    auto file = parse(idx, "cpp_enum.cpp", code);
    const cpp_entity* alias = nullptr;
    const cpp_entity* class_decl = nullptr;
    std::size_t count = test_visit<cpp_type_alias>(*file, [&](const cpp_type_alias& entity)
    {
        alias = &entity;
    });
    count += test_visit<cpp_class>(*file, [&](const cpp_class& entity)
    {
        class_decl = &entity;
    });

    REQUIRE(alias != nullptr);
    REQUIRE(class_decl != nullptr);

    bound_nodes_map map;

    REQUIRE(!map.get("foo").has_value());
    REQUIRE(map.get_all("foo").empty());

    map.bind("alias", *alias);
    REQUIRE(map.get("alias").has_value());
    REQUIRE(map.get("alias").value().has_value());
    REQUIRE(map.get("alias").value().as<cpp_type_alias>().value().name() == "mytypedef");
    REQUIRE(map.get_all("alias").size() == 1);
    REQUIRE(map.get_all("alias").front().as<cpp_type_alias>().value().name() == "mytypedef");
    REQUIRE_NOTHROW(map.get_as<cpp_type_alias>("alias").value().name() == "mytypedef");
}
